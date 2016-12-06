<?php
import("com.laurus.core.modelconfig.ModelConfig");
import("com.laurus.core.exception.ValidationException");
import("com.laurus.core.services.CommandValidater");
import("com.laurus.core.exception.MergeMoreActionsException");
import("com.laurus.core.json.JsonObjectWriter");
import("com.laurus.core.json.JSonFileUtil");

class Controller {
    private static $controller;
    private $wrapperRequest;
    private $actionLocator;
    protected $logger;
    const EVENT_FILE="controller-events.json";
    private $isEventFileLoaded;
    public static function getInstance() {
        if(!self::$controller) {
            self::$controller=new Controller();
        }
        return self::$controller;
    }
    public function setCommandConfig(ModelConfig $modelConfig) {
        $this->actionLocator=$modelConfig;
        $this->actionLocator->getActions();
        if($this->actionLocator==null) {
            $this->logger->fatal("Model Config could not be loaded");
            throw new Exception("Model Config could not be loaded");
        }
    }
    //giving sigleton behavior
    private function  __construct() {
        $this->logger=Logger::getLogger(__CLASS__);
        $this->isEventFileLoaded=false;
        try{
            JSonFileUtil::getJSONFromFile(self::EVENT_FILE);
            $this->isEventFileLoaded=true;
        }  catch (FileException $ex){
            $this->logger->warn("No Event file present for controller");
        }
    }
    public function setWrapperRequest() {
        $this->wrapperRequest=new WrapperRequest($_REQUEST, $_SESSION);
    }
    private function manageRequestData(){
        if(isset($_SESSION["JSON_RESPONSE_BEAN"])){
            $_SESSION["JSON_RESPONSE_BEAN"]=null;
        }
    }
    public function execute() {
        $this->manageRequestData();
        $this->controllerExecutionStarted();
        if(isset($_REQUEST["command"])) {
            $commandName=$_REQUEST["command"];
            if($this->actionLocator==null) {
                throw new Exception("Model Config was null.Controller Must know the config class");
            }
            $actions=$this->actionLocator->isCommandAvailable($commandName);
            try{ 
                $commandValidator=  CommandValidater::getInstance(); //this may throw file not found
                $actions=$commandValidator->prependCommandTasks($commandName, $actions); // Prepending actions for the validations
                $actions=$commandValidator->appendCommandTasks($commandName, $actions); //appending actions for post executions
            }
            catch (FileException $ex){
                //omit
            }
            
            
           
            
            $this->executeActions($actions);
        }
        $this->controllerExecutionCompleted();
    }
    
    private function executeActions($actions){
        if($actions!=false) {
            foreach ($actions as $key=>$value) {
                import($value);
                $actionName=substr(strrchr($value,"."),1);
                $this->logger->info("Action Called : " . $actionName);
                $actionObject=new $actionName;
                try {
                    $result=$actionObject->invoke();
                }catch (MergeMoreActionsException $ex1){
                    $newCommand=$_REQUEST["SMVC_NEW_MERGED_COMMAND"];
                    if($newCommand){
                        $newActions=$this->actionLocator->isCommandAvailable($newCommand);
                        $this->executeActions($newActions);
                    }
                }catch(ValidationException $e) {
                   $this->logger->debug($e->getTraceAsString());
                   $_REQUEST["ERROR_CONTAINER"]=$e->getMessage();
                   $_REQUEST["ERROR_CODE"]=$e->getCode();
                   return;
                }catch (Exception $e) {
                   $this->logger->debug($e->getTraceAsString());
                   $_REQUEST["ERROR_CONTAINER"]=$e->getMessage();
                   $_REQUEST["ERROR_CODE"]=$e->getCode();
                   return;
                }
                $_REQUEST[$_REQUEST["command"]][$key]=serialize($result);
                $_SESSION[$_REQUEST["command"]][$key]=serialize($result);
            }
        }
        
    }
    
    protected function controllerExecutionCompleted(){
        if(Application::getInstance()->getApplicationConfig()->isPureMVC()){
                Application::getInstance()->renderView();
        }
    }
    
    protected function controllerExecutionStarted(){
        if(Application::getInstance()->getApplicationConfig()->isPureMVC()){
                Application::getInstance()->generateURLConfig();
        }
    }
}

?>
