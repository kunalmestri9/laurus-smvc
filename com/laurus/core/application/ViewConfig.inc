<?php
/**
 * Description of ViewConfig
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.collection.SimpleList");
import("com.laurus.core.collection.map.Map");
import("com.laurus.core.application.UrlConfig");

class ViewConfig {
    /**
     *
     * @var SimpleList 
     */
    private $simpleUrlPatterns;
    /**
     *
     * @var SimpleList 
     */
    private $complexUrlPatterns;
    /**
     *
     * @var Map 
     */
    private $regExPatterns;
    
    private $logger;
    
    public function __construct($configObject) {
        $this->logger=Logger::getLogger(__CLASS__);
        $this->simpleUrlPatterns=new SimpleList();
        $this->complexUrlPatterns=new SimpleList();
        $this->regExPatterns=new Map();
        $this->regExPatterns->put("all", "/^([a-zA-Z0-9_\-\=])+$/");
        $this->regExPatterns->put("string","/^([a-zA-Z])+$/");
        $this->regExPatterns->put("int", "/^([0-9])+$/");
        $this->parseConfig($configObject);
    }

    protected function parseConfig($configObject){
        $this->logger->debug($configObject);
        foreach ($configObject as $value) {
            $this->logger->debug($value);
            $url=$value["url"];
            $this->logger->debug($url);
            if($this->isComplexUrl($url)){
                $this->logger->debug("This is Complex URL");
                $this->complexUrlPatterns->add($this->getComplexUrlConfig($value));
            }else{
                $this->logger->debug("This is simple URL");
                $tmpl=$value["tmpl"];
                $urlConfig=new UrlConfig($url, $tmpl);
                if(isset($value["isAbsolute"]) && TUtilString::equals("true", $value["isAbsolute"])){
                    $urlConfig->setIsAbsolute(true);
                }
                if(isset($value["command"])){
                    $urlConfig->setCommand($value["command"]);
                }
                $this->simpleUrlPatterns->add($urlConfig);
            }
        }
    }
    private function isComplexUrl($url){
        return TUtilString::contains($url,"{");
    }
    
    private function getComplexUrlConfig($value){
        $url=$value["url"];
        $tmpl=$value["tmpl"];
        $urlconfig=new UrlConfig($url, $tmpl);
        if(isset($value["isAbsolute"]) && TUtilString::equals("true", $value["isAbsolute"])){
            $urlconfig->setIsAbsolute(true);
        }
        if(isset($value["command"])){
            $urlconfig->setCommand($value["command"]);
        }
        $regExMap=new Map();
        $urlArray=explode("/", $url);
        foreach ($urlArray as $v) {
            if($this->isComplexUrl($v)){
                $pattern=substr($v, 1, (count($v)-1));
                $patternKeyVal=explode(":", $pattern);
                $regExMap->put($patternKeyVal[0], $patternKeyVal[2]);
            }
        }
        $urlconfig->setRegExMap($regExMap);
        $customRegExPatterns=$value["customTypes"];
        if($customRegExPatterns!=null){
            $customRegExMap=new Map();
            foreach ($customRegExPatterns as $key=>$value) {
                $customRegExMap->put($key, $value);
            }
            $urlconfig->setCustomRegExType($customRegExMap);
        }
        return $urlconfig;
    }
    
    public function getSimpleUrlPatterns() {
        return $this->simpleUrlPatterns;
    }

    public function setSimpleUrlPatterns(SimpleList $simpleUrlPatterns) {
        $this->simpleUrlPatterns = $simpleUrlPatterns;
    }

    public function getComplexUrlPatterns() {
        return $this->complexUrlPatterns;
    }

    public function setComplexUrlPatterns(SimpleList $complexUrlPatterns) {
        $this->complexUrlPatterns = $complexUrlPatterns;
    }

    public function getRegExPatterns() {
        return $this->regExPatterns;
    }

    public function setRegExPatterns(Map $regExPatterns) {
        $this->regExPatterns = $regExPatterns;
    }


}

?>
