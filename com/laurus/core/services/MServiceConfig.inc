<?php
import("com.laurus.core.services.IServiceConfig");
/**
 * Description of MServiceConfig
 *
 * @author kunal.mestri
 */
class MServiceConfig implements IServiceConfig{
    protected $config;
    public function __construct($array) {
        $this->config=$array;
    }
    
    public function getConfigParameter($parameterName) {
        return $this->config[$parameterName];
    }    
}

?>
