<?php
/**
 * Description of ServiceManager
 *
 * @author kunal.mestri
 */
interface IServiceManager {
    const SERVICE_CONFIG_FILE="service-config.json";
    const PROJECT_SERVICE_CONFIG_FILE="project-service-config.json";
    public function loadServiceConfiuguration();
    public function initAllServices();
}

?>
