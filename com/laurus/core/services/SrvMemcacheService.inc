<?php
/**
 * SrvMemcacheService contains the memcache related data .
 * Mostly it will take care of the object which will be used Application wise 
 *
 * @author kunal.mestri
 */
class SrvMemcacheService {
    private static $logger;
    private static $instance;
    public  static function getInstance($param) {
        if(self::$instance==null){
            self::$instance=new SrvMemcacheService();
        }
        return self::$instance;
    }
    private function __construct() {
        self::$logger=  Logger::getLogger(__CLASS__);
    }
}

?>
