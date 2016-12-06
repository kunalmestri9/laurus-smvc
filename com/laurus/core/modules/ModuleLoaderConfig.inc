<?php

/**
 * Description of ModuleLoaderConfig
 * This will read module.json file and will work as SingleTon Configuration for Whole ModuleLoader
 * @author kunal.mestri
 */
import("com.laurus.core.filters.FilterExecutor");
class ModuleLoaderConfig {
    protected static $instance;
    protected static $MODULE_LOADER_CONFIG_FILE= "module.json";
    protected static $logger;
    
    protected static $config;


    protected static $viewConfig;
    protected static $pageConfig;
    protected static $projectServiceConfig;

    protected static $moduleConfigRead;
    
    
    public function isConfigPresent(){
        return self::$moduleConfigRead;
    }
    
    public function getModuleLoaderConfig(){
        return self::$config;
    }
    
    //not allowing object creation from outside
    protected function __construct() {
        self::$logger=  Logger::getLogger(__CLASS__);
        self::$moduleConfigRead=true;
        $this->readJsonModuleLoaderConfig();
    }
    /**
     * 
     * @return ModuleLoaderConfig
     */
    public static function getInstance(){
        if(self::$instance==null){
            self::$instance=new ModuleLoaderConfig();
            self::$logger->info("ModuleLoaderConfig Instanciated");
        }
        return self::$instance;
    }


    private function getModuleConfigPath(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$MODULE_LOADER_CONFIG_FILE;
    }

    protected function readJsonModuleLoaderConfig(){
        $moduleConfigFilename=$this->getModuleConfigPath();
        if(!file_exists($moduleConfigFilename)){
            self::$logger->warn($moduleConfigFilename ."module.json not found in the application");
            self::$moduleConfigRead=false;
        } 
        self::$config=json_decode(file_get_contents($moduleConfigFilename),true);
    }
    

    protected function readViewConfigFromModule(\Module $module){

    }

    protected function readPageConfigFromModule(\Module $module){

    }

    protected function readProjectServiceConfigFrom(\Module $module){

    }

    
}

?>
