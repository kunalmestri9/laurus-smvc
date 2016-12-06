<?php
class BeanAttr{
    private $key;
    private $val;
    private $type;
    private $propertyRef;
    private $compositeType;
    private $returnType;
    public function getReturnType() {
        return $this->returnType;
    }

    public function setReturnType($returnType) {
        $this->returnType = $returnType;
    }

            

    public function getCompositeType() {
        return $this->compositeType;
    }

    public function setCompositeType($compositeType) {
        $this->compositeType = $compositeType;
        return $this;
    }

    
    public function setPropertyRefference($propertyRef){
        $this->propertyRef=$propertyRef;
        return $this;
    }
    
    public function getPropertyRefference(){
        return $this->propertyRef;
    }


    public function setKey($key){
        $this->key=$key;
        return $this;
    }
    
    public function getKey(){
        return $this->key;
    }

    public function setVal($val){
        $this->val=$val;
        return $this;
    }
    
    public function getVal(){
        return $this->val;
    }


    public function setType($type){
        $this->type=$type;
        return $this;
    }
    
    public function getType(){
         return $this->type;
    }   



}

?>
