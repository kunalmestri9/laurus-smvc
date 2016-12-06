<?php
/**
 *  Yes Indeed this class will serve as Module Loader to combile multiple webapps as per the request made
 *
 * @author kunal.mestri
 */
import("com.laurus.core.services.IBootStrap");
import("com.laurus.core.modules.ModuleLoaderConfig");
import("com.laurus.core.collection.SimpleList");
import("com.laurus.core.collection.map.Map");
import("com.laurus.core.modules.SMVCModule");



class ModuleLoader implements IBootStrap {
   protected $logger;
   protected static $instance;
   /**
    *
    * @var SimpleList 
    */
   protected $list;
   
   /**
    *
    * @var Map 
    */
   protected $map;
   /**
    *
    * @var ModuleLoaderConfig 
    */
   protected static $config;
   /**
    * 
    * @return ModuleLoader
    */
   public static function getInstance(){
       if(self::$instance==null){
            self::$instance=new ModuleLoader();
       }
       return self::$instance;
   }
   protected function __construct() {
       $this->logger=Logger::getLogger(__CLASS__);
   }
   public function init(){
       $this->logger->info("Starting Module Loader");
       $this->loadModuleLoaderConfig();
   }
   
   protected function loadModuleLoaderConfig(){
       $this->logger->debug("Loading Module Config");
       
       self::$config=ModuleLoaderConfig::getInstance();
       if(self::$config->isConfigPresent()){
           $configArray=self::$config->getModuleLoaderConfig();
           if(isset($configArray["moduleList"])){
               $this->logger->debug("Module List Loaded");
               $this->createModuleList($configArray["moduleList"]);
           }
            
       }
   }
   
   public function getConfig(){
      return self::$config;
   }
   
   protected function createModuleList($moduleArray){
       $this->list=new SimpleList();
       $this->map=new Map();
       foreach ($moduleArray as $key=>$module){
           $moduleObject=new SMVCModule();
           $module["name"]=$key;
           $moduleObject->updateObjectFromArray($module);
           $this->list->add($moduleObject);
           $this->map->put($key, $moduleObject);
       }
   }
   
   public function getAllModules(){
       return $this->list;
       
   }
   
   public  function isModulePresent($name){
       return $this->map->contains($name);
   }
   
   public function getModuleByModuleName($name){
       return ($this->map->contains($name))?$this->map->get($name)->getValue():"";
   }
   
}
?>
