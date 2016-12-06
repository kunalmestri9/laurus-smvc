<?php

/**
 * Description of ViewSelector
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.application.PageConfig");

class PageSelector {

    private $pageConfig;
    private $logger;

    public function __construct(PageConfig $config) {
        $this->pageConfig = $config;
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function setPage() {
        $map = $this->pageConfig->getPageMap();
        if ($map->contains($_REQUEST['currentPage']))
            $_REQUEST["SMVC_INNER_PAGE"] = $map->get($_REQUEST['currentPage'])->getValue();
        else if ($map->contains("page_not_found"))
            $_REQUEST["SMVC_INNER_PAGE"] = $map->get("page_not_found")->getValue();
    }

}

?>
