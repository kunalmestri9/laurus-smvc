<?php
/**
 *  Yes Indeed this class will serve as application Class will manage All the services that the Application Must be having
 *
 * @author kunal.mestri
 */
import("com.laurus.core.services.IBootStrap");
import("com.laurus.core.filters.FilterExecutor");
import("com.laurus.core.application.ApplicationConfig");


class Application implements IBootStrap {
   private $logger;
   private static $instance;
   /**
    *
    * @var ApplicationConfig 
    */
   private static $config;
   public static function getInstance(){
       if(self::$instance==null){
            self::$instance=new Application();
       }
       return self::$instance;
   }
   private function __construct() {
       $this->logger=Logger::getLogger(__CLASS__);
   }
   public function init(){
       $this->logger->info("Starting Application");
       $this->loadApplicationConfig();
       $this->startFilterService();
   }
   
   protected function loadApplicationConfig(){
       self::$config=ApplicationConfig::getInstance();
   }
   
   private function startFilterService(){
       $filterExecuter=new FilterExecutor();
       $filterExecuter->setFiltersConfiguration(self::$config->getFilterConfig());
       $filterExecuter->executeFilters();
   }
   
   
}

?>
