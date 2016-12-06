<?php
/**
 *  Yes Indeed this class will serve as application Class will manage All the services that the Application Must be having
 *
 * @author kunal.mestri
 */
import("com.laurus.core.services.IBootStrap");
import("com.laurus.core.filters.FilterExecutor");
import("com.laurus.core.application.ApplicationConfig");
import("com.laurus.core.application.ViewSelector");
import("com.laurus.core.application.PageSelector");


class Application implements IBootStrap {
   private $logger;
   private static $instance;
   /**
    *
    * @var ApplicationConfig 
    */
   private static $config;
   /**
    * 
    * @return Application
    */
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
   
   public function getApplicationConfig(){
       return self::$config;
   }
   
   public function renderView(){
       $this->logger->debug("RenderView  Called");
       $viewConfig=self::$config->getViewConfig();
       $pageConfig=self::$config->getPageConfig();
       $viewSelector=new ViewSelector(new ViewConfig($viewConfig));
       $pageSelector=new PageSelector(new PageConfig($pageConfig));
       $pageSelector->setPage();
       $viewSelector->renderView();
       $this->logger->debug("RenderView  completed");
   }
   
   public function generateURLConfig(){
       $this->logger->debug("URLConfig called");
       $viewConfig=self::$config->getViewConfig();
       $viewSelector=new ViewSelector(new ViewConfig($viewConfig));
       $cfg=$viewSelector->generateURLConfig(new HttpRequest());
       $_REQUEST["REQUEST_URLCONFIG"]=$cfg;
       $this->logger->debug("URLConfig completed");
   }
}
?>
