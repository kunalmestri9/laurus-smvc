<?php
/**
 * Description of ExecuterLoader
 *
 * @author Bhupesh
 */
import("com.laurus.core.json.JSonFileUtil");
import("com.laurus.core.services.ClassLoader");
class JobExecuterLoader {
    private $logger;
    private static $JOB_CONFIG_FILE= "jobs.json";
    private static $CFG_FOLDERNAME= "cfg";
    private $CONFIG;
    public function __construct(){
        $this->logger=Logger::getLogger(__CLASS__);
    }
    public function loadExecuterFromFile(){
        try{
            $this->logger->debug("Jobs config file path : ".$this->getJobConfigPath());
            $this->CONFIG=JSonFileUtil::getJSONFromFile($this->getJobConfigPath());
            $this->logger->debug("Jobs config Array");
            $this->logger->debug($this->CONFIG);
        }  catch (FileException $ex){
            $this->logger->warn("no job configuration file present");
        }
    }
    private function getJobConfigPath(){
        return  $_SERVER["APPLICATION_PATH"] . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$JOB_CONFIG_FILE;
    }
    public function runJobExecuters(){
        foreach ($this->CONFIG as $key => $value) {
            $this->executeSingleJob($key, $value);
        }
    } 
    
    
    public function runJob($key,$object){
        if(isset($this->CONFIG[$key])){
            $this->executeSingleJob($key, $this->CONFIG[$key],$object);
        }
    }
    protected function executeSingleJob($key,$value,$object=false){
        $this->logger->debug("Loading jobs : " . $key );
        if(!isset($value["class"])){
            $this->logger->error($key ." job class name is missing");
        }
        $this->logger->debug($value["class"]);
        $obj=ClassLoader::loadName($value["class"])->getInstance();
        try {
            $this->logger->debug($obj);
            $this->logger->debug($object);
            if($obj instanceof IJobExecuter)$obj->executeJobs($object);
        }catch (Exception $e) {
           $this->logger->debug($e); 
           $this->logger->debug($e->getTraceAsString());
        }
    }
    
    
    
    
    
}
?>
