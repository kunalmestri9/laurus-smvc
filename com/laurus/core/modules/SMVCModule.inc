<?php

/**
 * Description of SMVCModule
 * SMVC module represents to any module that has been added to the webapp
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class SMVCModule {
    private $moduleName;
    private $modelConfigFolder;
    private $modelConfigFile;
    private $tqueryListFilePath;
    private $modulePath;
    private $moduleURL;
    
    public function __construct() {
    
    }
    
    public function getModuleName() {
        return $this->moduleName;
    }

    public function setModuleName($moduleName) {
        $this->moduleName = $moduleName;
    }

    public function getModelConfigFolder() {
        return $this->modelConfigFolder;
    }

    public function setModelConfigFolder($modelConfigFolder) {
        $this->modelConfigFolder = $modelConfigFolder;
    }

    public function getModelConfigFile() {
        return $this->modelConfigFile;
    }

    public function setModelConfigFile($modelConfigFile) {
        $this->modelConfigFile = $modelConfigFile;
    }

    public function getTqueryListFilePath() {
        return $this->tqueryListFilePath;
    }

    public function setTqueryListFilePath($tqueryListFilePath) {
        $this->tqueryListFilePath = $tqueryListFilePath;
    }

    public function getModulePath() {
        return $this->modulePath;
    }

    public function setModulePath($modulePath) {
        $this->modulePath = $modulePath;
    }

    public function getModuleURL() {
        return $this->moduleURL;
    }

    public function setModuleURL($moduleURL) {
        $this->moduleURL = $moduleURL;
    }

    public function updateObjectFromArray($moduleObject){
        $this->setModelConfigFile($moduleObject["modelConfigFile"]);
        $this->setModelConfigFolder($moduleObject["modelConfigFolder"]);
        $this->setModuleName($moduleObject["name"]);
        $this->setModulePath($moduleObject["path"]);
        $this->setModuleURL($moduleObject["url"]);
        $this->setTqueryListFilePath($moduleObject["tqueryListPath"]);
    }


}

?>
