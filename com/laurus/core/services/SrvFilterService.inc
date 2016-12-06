<?php
/**
 * @author Kunal Mestri <kunalmestri9@gmail.com>
 * 
 */
import("com.laurus.core.filters.IFilter");
import("com.laurus.core.collection.map.Map");

class SrvFilterService {
    private static $filters;
    private static $instance;
    private function __construct() {
        self::$filters=new Map();
    }
    public static function getInstance() {
        if(self::$instance==null){
           self::$instance=new SrvFilterService();
        }
        return self::$instance;
    }
    public function getFilter($filtername) {
        return self::$filters->get($filterName);
    }
    public function putFilter($filterName,IFilter $filterObject){
         self::$filters->put($filterName,$filterObject);
    }
}

?>
