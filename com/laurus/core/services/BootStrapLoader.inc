<?php
import("com.laurus.core.services.IBootStrap");
import("com.laurus.core.services.ClassLoader");


/**
 * Description of BootStrapLoader
 *
 * @author kunal.mestri
 */
class BootStrapLoader implements IBootStrap{
    private static $logger;
     const CORE_OBJECT_FILE="core-objects.json";
    public function __construct(){
        self::$logger=Logger::getLogger(__CLASS__);
    }
    
    public function startBootStrap(){
        foreach ($this->globalConfiguration as $key=>$value) {
            self::$logger->debug("Loading Bootstrap : " . $value );
            import($value);
            $bootStrapClass=substr(strrchr($value,"."),1);
            $bootStrap=$bootStrapClass::getInstance();
            try{
                $bootStrap->init();
            }catch (Exception $e){
                 self::$logger->error($key ."  init failed");
                 self::$logger->error($e);
            }
            self::$logger->debug("BooStrap initialized : " . $key );
        }
        self::$logger->debug("BootStrap Loader finished loading services");
    }
    
     public function loadServiceConfiuguration() {
        if(!file_exists($this->getConfigurationFile())){
            self::$logger->error("core-object config not found");
            return;
        }
        self::$logger->debug(file_get_contents($this->getConfigurationFile()));
        $this->globalConfiguration=json_decode(file_get_contents($this->getConfigurationFile()),true);
        if($this->globalConfiguration==null || !isset($this->globalConfiguration["bootstrap"])){
            self::$logger->error("core-object config does  not contain valid JSON config");
            return;
        }
        self::$logger->debug( $this->globalConfiguration);
        $this->globalConfiguration=$this->globalConfiguration["bootstrap"];
        self::$logger->debug( $this->globalConfiguration);
        $this->startBootStrap();
    }    
    
    protected function getConfigurationFile(){
        return $_SERVER["APPLICATION_PATH"] . "cfg" . DIRECTORY_SEPARATOR . BootStrapLoader::CORE_OBJECT_FILE;
    }

    public function init() {
        $this->loadServiceConfiuguration();
    }
}

?>
