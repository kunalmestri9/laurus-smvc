<?php
abstract class LCoreBean{
    /**
     * @blockedJson
     * @var type 
     */
    protected $appendQuery;
    /**
     * @blockedJson
     * @var type 
     */
    protected $selectionColumn;
    public function getSelectionColumn() {
        return $this->selectionColumn;
    }

    public function setSelectionColumn($selectionColumn) {
        $this->selectionColumn = $selectionColumn;
    }

    public function getAppendQuery() {
        return $this->appendQuery;
    }

    public function setAppendQuery($appendQuery) {
        $this->appendQuery = $appendQuery;
    }

    

    public function toJson($isMapRequired=false) {
        $map=new Map();
        $cl1=get_class($this);
        $ref=new ReflectionClass($cl1);
         foreach ($ref->getProperties() as $refProperty) {
            $val=$this->getValueOfProperty($ref,$refProperty,$this); 
            if($val!=""){
                 if (preg_match('/@blockedJson\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                     //Do nothing its not allowed to render
                 }else{
                     if(preg_match('/@jsonDate\s+([^\s]+)/', $refProperty->getDocComment(), $matches)){
                         $map->put($refProperty->getName(),TUtilDate::getMilisecondsFromDate($val));
                     }else{
                         $map->put($refProperty->getName(), $val);
                     }
                     
                 }
                 
                 
            }
               
        }
        return (!$isMapRequired)? $map->toJson():$map ;
    }
    
    private function getValueOfProperty($refClass,$refProperty,$bean){
        if($refProperty->isPublic()){
            $val=$refProperty->getValue($bean);
        }else{
            $methodName="get" . ucfirst($refProperty->getName());
            if($refClass->hasMethod($methodName)){
                $refMethod=$refClass->getMethod($methodName);
                $val=$refMethod->invoke($bean);
            }else{
                //throw exception that method not available for private/protected property
            }
        }
        return $val;
    }

}

?>
