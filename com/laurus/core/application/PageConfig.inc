<?php
/**
 * Description of PageConfig
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.collection.SimpleList");
import("com.laurus.core.collection.map.Map");

class PageConfig {
    private $pageConfig;
    private $logger;
    
    public function __construct($configObject) {
        $this->logger=Logger::getLogger(__CLASS__);
        $this->pageConfig=new Map();
        $this->parseConfig($configObject);
    }

    protected function parseConfig($configObject){
        $this->logger->debug($configObject);
        if(is_array($configObject)){
            foreach ($configObject as $key => $value) {
                $this->pageConfig->put($key,$value);
            }
        }
    }
    public function getPageMap() {
        return $this->pageConfig;
    }
}

?>
