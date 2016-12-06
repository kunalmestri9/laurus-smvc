<?php

/**
 * Description of ViewSelector
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.application.ViewConfig");

class ViewSelector {

    private $viewConfig;
    private $logger;

    public function __construct(ViewConfig $config) {
        $this->viewConfig = $config;
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function generateURLConfig(HttpRequest $wr) {
        $url = $wr->getURI();
        $this->logger->debug($url);
        /* @var $urlConfig UrlConfig */
        $urlConfig = $this->matchSimpleUrls($url);
        if ($urlConfig == null) {
            $this->logger->debug("Rendering using URLConfig");
            $urlConfig = $this->matchComplexUrls($url);
            if ($urlConfig == null) {
                return;
            }
        }
        return $urlConfig;
    }

    public function renderView() {
        if (isset($_REQUEST["REQUEST_URLCONFIG"]))
            $this->renderTemplate($_REQUEST["REQUEST_URLCONFIG"]);
    }

    private function renderTemplate(UrlConfig $urlConfig) {
        $this->logger->debug($_SERVER["TMPL_PATH"] . $urlConfig->getTemplate());
        if($urlConfig->getIsAbsolute()){
            require_once $urlConfig->getTemplate();
        }else{
            require_once $_SERVER["TMPL_PATH"] . $urlConfig->getTemplate();
        }
        
    }

    private function matchSimpleUrls($url) {
        /* @var $list SimpleList */
        $list = $this->viewConfig->getSimpleUrlPatterns();
        $this->logger->debug($list);
        for ($i = 0; $i < $list->size(); $i++) {
            /* @var $urlConfig UrlConfig */
            $urlConfig = $list->get($i);
            $requestMap = new Map();
            if (TUtilString::equals($urlConfig->getUrl(), $url)) {
                return $this->settingUrlConfigForRequest($urlConfig, $requestMap);
            }
            $this->logger->debug($urlConfig->getUrl() . "!=" . $url);
        }

        return null;
    }

    private function addRequestMapToRequest(Map $requestMap) {
        $this->logger->debug("Add to request map called");
        $iterator = $requestMap->getIterator();
        while ($iterator->hasNext()) {
            /* @var $entry Entry */
            $entry = $iterator->next();
            $this->logger->debug($entry);
            $_REQUEST[$entry->getKey()] = $entry->getValue();
        }
    }

    private function settingUrlConfigForRequest(UrlConfig $urlConfig, Map $requestMap) {
        $this->logger->debug("Returning urlConfig1");
        $this->addRequestMapToRequest($requestMap);
        if ($urlConfig->isCommandPresent()) {
            $_REQUEST["command"] = $urlConfig->getCommand();
        }
        return $urlConfig;
    }

    private function matchComplexUrls($url) {
        $urlArray = explode("/", trim($url, "/"));
        /* @var $list SimpleList */
        $list = $this->viewConfig->getComplexUrlPatterns();
        $this->logger->debug($list);

        for ($i = 0; $i < $list->size(); $i++) {
            /* @var $urlConfig UrlConfig */
            $urlConfig = $list->get($i);
            $requestMap = new Map();
            $templateUrlArray = explode("/", trim($urlConfig->getUrl(), "/"));
            if (count($templateUrlArray) != count($urlArray)) {
                continue 1;
            }
            for ($j = 0; $j < count($templateUrlArray); $j++) {
                if (TUtilString::equals($templateUrlArray[$j], $urlArray[$j])) {
                    $this->logger->debug($j . "==" . (count($templateUrlArray) - 1));
                    if ($j == (count($templateUrlArray) - 1)) {
                        return $this->settingUrlConfigForRequest($urlConfig, $requestMap);
                    }
                    continue;
                }
                if (TUtilString::contains($templateUrlArray[$j], "{")) {
                    $templateUrlArrayString = substr($templateUrlArray[$j], 1, strlen($templateUrlArray[$j]) - 2);
                    $regEx = explode(":", $templateUrlArrayString);
                    $this->logger->debug($regEx[1]);
                    /* @var $regExPattern Entry */
                    $regExPattern = null;
                    if ($this->viewConfig->getRegExPatterns()->contains($regEx[1])) {
                        $regExPattern = $this->viewConfig->getRegExPatterns()->get($regEx[1]);
                    } else if ($urlConfig->getCustomRegExType()->contains($regEx[1])) {
                        $regExPattern = $urlConfig->getCustomRegExType()->get($regEx[1]);
                    }
                    if ($regExPattern == null) {
                        throw new Exception("There is problem with the configuration");
                    }
                    $this->logger->debug($regExPattern->getValue() . "=>" . $urlArray[$j]);
                    $result = preg_match_all($regExPattern->getValue(), $urlArray[$j], $matches);
                    $this->logger->debug("RegEx Match Result : " . $result);
                    if ($result !== false && $result != 0) {
                        $requestMap->put($regEx[0], $urlArray[$j]);
                        $this->logger->debug($j . "==" . (count($templateUrlArray) - 1));
                        if ($j == (count($templateUrlArray) - 1)) {
                            return $this->settingUrlConfigForRequest($urlConfig, $requestMap);
                        }
                        continue;
                    }
                }
                continue 2;
            }
            $this->logger->debug($urlConfig->getUrl() . "!=" . $url);
        }
        return null;
    }

}

?>
