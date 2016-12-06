<?php

/**
 * Description of ApplicationConfig
 * This will read web.json file and will work as SingleTon Configuration for Whole Application
 * @author kunal.mestri
 */
import("com.laurus.core.filters.FilterExecutor");
class ApplicationConfig {
    private static $instance;
    private static $CFG_FOLDERNAME="cfg";
    private static $WEB_CONFIG_FILE= "web.json";
    private static $PAGE_CONFIG_FILE= "pageconfig.json";
    private static $logger;
    private static $isWebConfigRead=false;
    private static $config;
    //not allowing object creation from outside
    private function __construct() {
        self::$logger=  Logger::getLogger(__CLASS__);
        $this->readJsonWebConfig();
        if(class_exists(ModuleLoader)){
            $this->getExternalModuleConfiguration();
            self::$logger->debug("Final Loading-----------------------------------");
            self::$logger->debug(self::$config);
        }
        
    }
    /**
     * 
     * @return ApplicationConfig
     */
    public static function getInstance(){
        if(self::$instance==null){
            self::$instance=new ApplicationConfig();
            self::$logger->info("ApplicationConfig Instanciated");
        }
        return self::$instance;
    }
    
    public function getApplicationParameter($parameterName){
        $object=self::$config->get($parameterName);
        if($object){
            return $object->getValue();
        }
        return null;
    }
    public function getFilterConfig(){
        if($this->isAnyFilterAvailable()){
            return  self::$config["filters"];
        }
        return false;
    }
    public function getViewConfig(){
        if($this->isViewConfigAvailable()){
            return  self::$config["viewConfig"];
        }
        return false;
    }
    public function getPageConfig(){
        if($this->isPageConfigAvailable()){
            return  self::$config["pageConfig"];
        }
        return false;
    }
    private function isPageConfigAvailable(){
        return isset(self::$config["pageConfig"]) ? true: false;
    }
    private function isViewConfigAvailable(){
        return isset(self::$config["viewConfig"]) ? true: false;
    }
    private function isAnyFilterAvailable(){
        return isset(self::$config["filters"]) ? true: false;
    }
    private function readJsonWebConfig(){
        $webConfigFileName=$this->getWebConfigPath();
        if(!file_exists($webConfigFileName)){
            self::$logger->warn($webConfigFileName ."web.json not found in the application");
            self::$isWebConfigRead=false;
        } 
        self::$config=json_decode(file_get_contents($webConfigFileName),true);
        $pageConfigFileName=$this->getPageConfigPath();
        if(file_exists($pageConfigFileName)){
            $arr=json_decode(file_get_contents($pageConfigFileName),true);
            self::$config["pageConfig"]=$arr;
        }else{
            self::$logger->warn($pageConfigFileName ."pageconfig.json not found in the application");
        }
        
        self::$logger->info("Config read from web.json");
        self::$logger->info("Config read from pageconfig.json");
        
        self::$logger->debug(self::$config);
    }
    
    private function getWebConfigPath(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$WEB_CONFIG_FILE;
    }
    private function getPageConfigPath(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$PAGE_CONFIG_FILE;
    }
    public function isPureMVC(){
        return  isset($_SERVER["IS_PURE_MVC"]) && $_SERVER["IS_PURE_MVC"]=="true";
    }
    
    public function getExternalModuleConfiguration(){
        self::$logger->debug(__METHOD__);
        $list=  ModuleLoader::getInstance()->getAllModules();
        self::$logger->debug($list);
        for ($i=0 ; $i < $list->size() ; $i++ ) { 
            /* @var $module SMVCModule*/
            $module=$list->get($i);
            $webConfigFileName=$module->getModulePath() . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$WEB_CONFIG_FILE;
            $pageConfigFileName=$module->getModulePath() . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$PAGE_CONFIG_FILE;
            $this->readWebConfigIndividual($webConfigFileName,$pageConfigFileName);
        }
    }
    
    private function readWebConfigIndividual($webConfigFileName,$pageConfigFileName){
        if(!file_exists($webConfigFileName)){
            self::$logger->warn($webConfigFileName ."web.json not found in the application");
            self::$isWebConfigRead=false;
        } 
        $newConfig=json_decode(file_get_contents($webConfigFileName),true);
	$elementsToCheck=array("viewConfig","filters");
        foreach($elementsToCheck as $key){
            if (isset($newConfig[$key])) {
                if (isset(self::$config[$key])) {
                    self::$logger->debug("----------------------------");
                    self::$logger->debug(self::$config[$key]);
                    self::$logger->debug("----------------------------");
                    self::$logger->debug($newConfig[$key]);
                    self::$config[$key] = array_merge(self::$config[$key], $newConfig[$key]);
                    self::$logger->debug("----------------------------");
                    self::$logger->debug(self::$config[$key]);
                    self::$logger->debug("----------------------------");
                } else {
                    self::$config[$key] = $newConfig[$key];
                }
            }
	}
        if (file_exists($pageConfigFileName)) {
            $arr = json_decode(file_get_contents($pageConfigFileName), true);
            if($arr!=null){
                if (!isset(self::$config["pageConfig"])) {
                    self::$config["pageConfig"] = $arr;
                } else {
                    self::$config["pageConfig"] = array_merge($arr,self::$config["pageConfig"]);
                }
            }
        } else {
            self::$logger->warn($pageConfigFileName . "pageconfig.json not found in the application");
        }
    }
}

?>
