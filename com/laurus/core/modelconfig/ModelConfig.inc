<?php
class ModelConfig{
    protected $isCommadSet;
    protected $array;
    protected $loggerConfig;
    private static $instance;
    /*public abstract static function getInstance();*/
    //Removing as per the standards of php 5.3
    protected function  __construct() {
        $this->array=array();
        $this->loggerConfig=Logger::getLogger(__CLASS__);
        $this->loggerConfig->info("Starting Up Model Config");
        $this->isCommadSet=false;
    }
    public function  getActions(){
        $this->loggerConfig->info(__CLASS__ ."::". __METHOD__);
        $this->setCommands();
    }

    public  function setCommands(){
        $this->loggerConfig->info(__CLASS__ ."::". __METHOD__);
        $this->setCommandConfig();
        $this->isCommadSet=true;
    }
    public function isCommandAvailable($command){
        $this->loggerConfig->info(__CLASS__ ."::". __METHOD__);
        $this->loggerConfig->info("Searching for command : " . $command );
        foreach ($this->array as $key =>$value) {
            if($key==$command){
                $this->loggerConfig->info($command." command found");
                return $value;
            }
        }
        $this->loggerConfig->warn(" Command '".$command."' not found");
        $_REQUEST["ERROR_CONTAINER"]=" Command '".$command."' not found";
        return false;
    }
    public function setCommandConfig() {
        $this->readCommandConfigFromFile($_SERVER["MODELCONFIG_FILE"],$_SERVER["MODELCONFIG_FOLDER"]);
        if(class_exists(ModuleLoader)){
            $this->getModuleConfigFromModules();
        }
    }
    
    private function readCommandConfigFromFile($moduleConfigFilePath,$moduleConfigFolder){
        if($moduleConfigFilePath==null || !file_exists($moduleConfigFilePath)){
            $this->loggerConfig->info("You may need to add the modelconfig file");
            return;
        }
        $this->loggerConfig->info("reading Modelconfig".$moduleConfigFilePath);
        $this->loggerConfig->debug("calling loadfiles");
        $this->loadCommands(json_decode(file_get_contents($moduleConfigFilePath),true),$moduleConfigFolder);
        $this->loggerConfig->info($this->array);
    }
    
    private function loadCommands($array,$moduleConfigFolder){
        foreach ($array as $key => $value){
            if(strpos($key, "__FILE__") === 0){
                $this->loggerConfig->debug("__FILE__ key found ".$value["name"]);
                $filePath=$moduleConfigFolder. $value["name"];
                $this->loggerConfig->debug("FILE :".$filePath);
                $this->loadCommands(json_decode(file_get_contents($filePath),true),$moduleConfigFolder);
            }else{
                $this->array[$key]=$value;
            }
        }
    }
    public  static function getInstance() {
        if(self::$instance==null) {
            //echo "ModelConfig::Loaded";
            self::$instance=new ModelConfig();
        }
        return self::$instance;
    }
    
    public function getModuleConfigFromModules(){
        $this->loggerConfig->debug(__METHOD__);
       
        
        $list=  ModuleLoader::getInstance()->getAllModules();
        $this->loggerConfig->debug($list);
        for ($i=0 ; $i < $list->size() ; $i++ ) { 
            /* @var $module SMVCModule*/
            $module= $list->get($i);     
            $this->readCommandConfigFromFile($module->getModelConfigFile(), $module->getModelConfigFolder());
        }
    }
}
?>
