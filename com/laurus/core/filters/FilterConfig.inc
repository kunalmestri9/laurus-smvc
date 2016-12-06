<?php
/**
 * Description of FilterConfig
 * Store the configuration for the Filter
 * @author kunal.mestri
 */
class FilterConfig {
    private $configArray;
    public function __construct($array) {
        $this->configArray=$array;
    }
    public function getInitParameter($parameterName) {
        return $this->configArray[$parameterName];
    }
    public function getInitParameterNames() {
        return array_keys( $this->configArray);
    }
}

?>
