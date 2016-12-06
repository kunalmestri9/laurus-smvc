<?php

import("com.laurus.core.modelconfig.ModelConfig");
import("com.laurus.core.exception.ValidationException");
import("com.laurus.core.json.JSonFileUtil");

/**
 * Description of CommandValidater
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class CommandValidater {

    private static $CommandValidater;
    protected $logger;
    private static $CFG_FOLDERNAME="cfg";
    private static $COMMANDVALIDATOR_CONFIG_FILE= "commandValidator.json";
    private static $POST_EXECUTOR_CONFIG_FILE= "postcommandexecutor.json";
    
    
    public static function getInstance() {
        if (!self::$CommandValidater) {
            self::$CommandValidater = new CommandValidater();
        }
        return self::$CommandValidater;
    }

    private function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
        $this->isEventFileLoaded = false;
        try {
            $this->array = JSonFileUtil::getJSONFromFile($this->getCommandValidatorConfigFile());
        } catch (FileException $ex) {
            $this->logger->warn("No command validator file present for command validator");
        }
        
        try {
            $this->postExecutorArray = JSonFileUtil::getJSONFromFile($this->getPostExecutorConfigFile());
        } catch (FileException $ex) {
            $this->logger->warn("Post command executor files seems to be missing from the application");
        }
        
        
    }

    public function prependCommandTasks($command, $actions) {
        if ($this->array && is_array($this->array)) {
            foreach ($this->array as $key => $value) {
                $commandArr = $value["commandArray"];
                $executeAction = $value["executeAction"];
                $isFound=in_array($command, $commandArr);
                if($isFound){
                    $newActions = $executeAction + $actions;
                    $this->logger->debug($newActions);
                    return $newActions;
                }
            }
        }
        return $actions;
    }
    
    /**
     * This function will mainly be used for appending the actions in a single command. There is another way of doing this
     * by throwing MergeMoreActions exeception from any action. But that execution may lead to recursive execution.
     * This should be used for response altering or background job adding purposes.
     * @param String $command
     * @param Array $actions
     * @return Array
     */
    public function appendCommandTasks($command, $actions) {
        if ($this->postExecutorArray && is_array($this->postExecutorArray)) {
            foreach ($this->postExecutorArray as $key => $value) {
                $commandArr = $value["commandArray"];
                $executeAction = $value["executeAction"];
                $isFound=in_array($command, $commandArr);
                if($isFound){
                    $newActions = $actions + $executeAction;
                    $this->logger->debug($newActions);
                    return $newActions;
                }
            }
        }
        return $actions;
    }

    
    
    protected function getCommandValidatorConfigFile(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$COMMANDVALIDATOR_CONFIG_FILE;
    }
    
    protected function getPostExecutorConfigFile(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$POST_EXECUTOR_CONFIG_FILE;
    }
    
    
}

?>
