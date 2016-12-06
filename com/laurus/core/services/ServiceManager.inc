<?php
import("com.laurus.core.services.IServiceManager");
import("com.laurus.core.services.IBootStrap");
import("com.laurus.core.services.IServiceLocator");
import('com.laurus.core.collection.map.Map');
import("com.laurus.core.services.MServiceConfig");

/**
 * Description of ServiceManager
 * Here you can configure project specfic services as well as global services.
 * @author kunal.mestri
 */
class ServiceManager implements IServiceManager,  IBootStrap,  IServiceLocator{
    private static $logger;
    private static $instance;
    private $globalConfiguration;
    private $projectConfiguration;
    private $isProjectServiceConfiguratonLoaded;
    private /*Map*/ $serviceMap;
    
    private function __construct(){
        self::$logger=Logger::getLogger(__CLASS__);
        $this->serviceMap=new Map();
    }
    /**
     * 
     * @return ServiceManager
     */
    public static function getInstance(){
        if(self::$instance==null){
            self::$instance=new ServiceManager();
        }
        return self::$instance;
    }
    
    public function initAllServices() {
        foreach ($this->globalConfiguration as $key =>$value) {
            self::$logger->debug("Loading service : " . $key );
            if(!isset($value["class"])){
                self::$logger->error($key ." service class name is missing");
            }
            import($value["class"]);
            $bootClass=substr(strrchr($value["class"],"."),1);
            $serviceObject=new $bootClass;
            try{
                $serviceObject->init($value["config"]);
            }catch (ServiceException $e){
                 self::$logger->error($key ." service init failed");
                 self::$logger->error($e);
            }
            $this->serviceMap->put($key, $serviceObject);
            self::$logger->debug("Service initialized : " . $key );
        }
         self::$logger->debug("Service manager finished loading services");
    }

    public function loadServiceConfiuguration() {
        self::$logger->debug("Service config :" . $this->getConfigurationFile());
        if(file_exists($this->getConfigurationFile())){
            $fileContents=file_get_contents($this->getConfigurationFile());
            self::$logger->debug("contents :" . $fileContents);
            $this->globalConfiguration=json_decode($fileContents,true);
            self::$logger->debug($this->globalConfiguration);
            if($this->globalConfiguration==null){
                self::$logger->error("service-config not found");
            }else{
                $this->initAllServices();    
            }
        }else{
            self::$logger->error($this->getConfigurationFile() . " File is missing. Please Add it if you want to Use this feature.");
        }
        self::$logger->debug("Loading Project Service config :" . $this->getProjectServiceConfig());
        if(file_exists($this->getProjectServiceConfig())){
            $fileContents2=file_get_contents($this->getProjectServiceConfig());
        }else{
            self::$logger->error($this->getProjectServiceConfig() . " File is missing. Please Add it if you want to Use this feature.");
        }
        self::$logger->debug("contents :" . $fileContents2);
        $this->projectConfiguration=json_decode($fileContents2,true);
        self::$logger->debug($this->projectConfiguration);
        if($this->projectConfiguration==null){
            $this->isProjectServiceConfiguratonLoaded=false;
            self::$logger->error("Project service-config not found");
        }else{
            $this->isProjectServiceConfiguratonLoaded=true;
        }
    }    
    
    protected function readProjectServiceConfig($configurationPath){
        self::$logger->debug("projectConfiguration");
        self::$logger->debug($this->projectConfiguration);
        self::$logger->debug("Loading Project Service config :" . $configurationPath);
        if(file_exists($configurationPath)){
            $fileContents=file_get_contents($configurationPath);
        }else{
            self::$logger->error($configurationPath . " File is missing. Please Add it if you want to Use this feature.");
        }
        self::$logger->debug("contents :" . $fileContents);
        $projectConfiguration=json_decode($fileContents,true);
        if($projectConfiguration!=null){
            if(!$this->isProjectServiceConfiguratonLoaded){
                $this->projectConfiguration=$projectConfiguration;
                $this->isProjectServiceConfiguratonLoaded=true;
            }else{
                $this->projectConfiguration=array_merge($this->projectConfiguration,$projectConfiguration);
            }
        }
        self::$logger->debug("projectConfiguration");
        self::$logger->debug($this->projectConfiguration);
    }


   
    protected function getConfigurationFile(){
        return $_SERVER["APPLICATION_PATH"]."cfg".DIRECTORY_SEPARATOR.IServiceManager::SERVICE_CONFIG_FILE;
    }
    
    protected function getProjectServiceConfig(){
        return $_SERVER["APPLICATION_PATH"]."cfg".DIRECTORY_SEPARATOR.IServiceManager::PROJECT_SERVICE_CONFIG_FILE;
    }

    public function init() {
        $this->loadServiceConfiuguration();
        if(class_exists(ModuleLoader)){
            $this->getProjectServiceConfigFromModules();
        }
    }

    public function getService($serviceName) {
        self::$logger->debug("Searching for servicename: ".$serviceName);
        if($this->isProjectServiceConfiguratonLoaded){
            $serviceConfig=$this->projectConfiguration[$serviceName];
            if(!$serviceConfig){
                throw new ServiceNotFoundException();
            }
            self::$logger->debug("Service config found".$serviceConfig["class"]);
            import($serviceConfig["class"]);
            $serviceClass=substr(strrchr($serviceConfig["class"],"."),1);
            self::$logger->debug("Servic class: ".$serviceClass);
            $service=$serviceClass::getInstance();
            try{
                $service->init(new MServiceConfig($serviceConfig["config"]));
            }catch(Exception $e){
                self::$logger->debug("service can not be initialized");
            }
            return $service;
        }
    }
    
    public function getProjectServiceConfigFromModules() {
        self::$logger->debug(__METHOD__);
        $list = ModuleLoader::getInstance()->getAllModules();
        self::$logger->debug($list);
        for ($i = 0; $i < $list->size(); $i++) {
            /* @var $module SMVCModule */
            $module = $list->get($i);
            $projectServiceConfigPath = $module->getModulePath() . DIRECTORY_SEPARATOR . "cfg" . DIRECTORY_SEPARATOR . IServiceManager::PROJECT_SERVICE_CONFIG_FILE;
            $this->readProjectServiceConfig($projectServiceConfigPath);
        }
    }
    
}

?>
