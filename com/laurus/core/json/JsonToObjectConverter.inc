<?php
import("com.laurus.core.services.ClassLoader");
import("com.laurus.core.collection.map.Map");
import("com.laurus.core.collection.SimpleList");

/**
 * Description of JsonToObjectConverter
 *
 * @author kunal.mestri
 */
class JsonToObjectConverter {
    
    private $classMap;
    public function __construct() {
        $this->classMap=new Map();
    }
    
    public function addParamClass($paramKey,$class){
        $this->classMap->put($paramKey,$class);
    }
    
    public function convertToObject($json,$class){
        if(is_string($json)){
            $array=json_decode($json,true);
            //var_dump($array);
        }else{
            $array=$json;
        }
        if(is_string($class)){
            if(strpos($class,"<")!==FALSE){
                $this->addParamClass($class, $class);
                return $this->convertToCollection($array,$class);
            }
            $object=ClassLoader::loadName($class)->getInstance();
        }else if(is_object($class)){
            $object=$class;
        }
        if($object instanceof Iterable){
            $this->convertToCollection($array, "MAPKEY",$object);
        }
        foreach ($array as $key => $value) {
            $methodName="set".ucfirst($key);
            //echo "in1";
            if(method_exists($object,$methodName)){
                if(is_array($value)){
                    call_user_func(array($object,$methodName),$this->convertToCollection($value, $key));
                    continue;
                }
                call_user_func(array($object,$methodName),$value);
            }
        }
        return $object;
    }
    
    public function convertSimpleListOfObjects($fromString,$beansOf){
        if(is_string($fromString)){
            $array=json_decode($fromString,true);
            //var_dump($array);
        }else{
            $array=$fromString;
        }
        $refClass=new ReflectionClass($beansOf);
            
            
        $collection=new SimpleList();
        if($beansOf instanceof SimpleList){
            //Need to think of this not supported for now.
        }else{
            for ($j = 0; $j < count($array); $j++) {
                $instance=$refClass->newInstance();
                $collection->add($this->convertToObject($array[$j],$instance));
            }
        }
        return $collection;
    }
    
    /**$fromString
     * 
     * @param type $value
     * @param type $key
     * @param Map $collection
     * @return \SimpleList
     */
    private function convertToCollection($value,$key,$collection=null){
        if($this->classMap->contains($key)){
            $classInStr=$this->classMap->get($key)->getValue();
            if(strpos($classInStr,"<")!==FALSE){
                $firstClass=substr($classInStr,0,strpos($classInStr,"<"));
                $secondClass=substr($classInStr,strpos($classInStr,"<")+1,strpos($classInStr,">")-strpos($classInStr,"<")-1);
            }else{
                $firstClass=$classInStr;
            }
            
            
            $collection=ClassLoader::loadName($firstClass)->getInstance();
            if($collection instanceof ICollection){
                if($collection instanceof SimpleList){
                    if(is_array($value[0])|| is_object($value[0])){
                        for ($j = 0; $j < count($value); $j++) {
                            $collection->add($this->convertToObject($value[$j],$secondClass));
                        }
                    }else{
                        for($i=0;$i<count($value);$i++){
                            $collection->add($value[$i]);
                        }
                    }
                }else if($collection instanceof Iterable){
                    //Code for Map
                    if(is_array($value[0])|| is_object($value[0])){
                        for ($j = 0; $j < count($value); $j++) {
                            $collection->add($this->convertToObject($value[$j],$secondClass));
                        }
                    }else{
                        for($i=0;$i<count($value);$i++){
                            $collection->add($value[$i]);
                        }
                    }
                }
                
            }else{
                return $this->convertToObject($value,$firstClass);
            }
        }else if($key=="MAPKEY"){
            foreach ($value as $key => $val) {
                $collection->put($key, $val);
            }
            
        }else{
            $collection=new SimpleList();
            for($i=0;$i<count($value);$i++){
                $collection->add($value[$i]);
            }
        }
        return $collection;
     }
    
}

?>
