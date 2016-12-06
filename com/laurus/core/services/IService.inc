<?php
import("com.laurus.core.services.IServiceConfig");

/**
 * Description of IService
 *
 * @author kunal.mestri
 */
interface IService {
    public static function getInstance();
    public function init(IServiceConfig $config);
}

?>
