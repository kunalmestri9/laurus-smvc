<?php
import("com.laurus.core.coreent.LCoreBean");
abstract class AbstractMergerBean extends LCoreBean{
    
    public function toJson($isMapRequired=false) {
        $a=$this->getActualResponseBean();
        return $a->toJson($isMapRequired);
    }
    
    abstract public function getActualResponseBean();
     

}

?>
