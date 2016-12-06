<?php
import("com.laurus.core.coreent.LCoreBean");
import("com.laurus.core.collection.CollectionConverter");

abstract class AbstractMergerBeanCollection extends LCoreBean{
    protected $map;
    private $logger;
    protected $list;
    
    public function __construct(){
        $this->map=new Map();
        $this->list=new SimpleList();
        $this->logger=  Logger::getLogger(__CLASS__);
    }
    
    public function getList() {
        return $this->list;
    }

    public function setList($list) {
        $this->list = $list;
    }

        
    abstract public function getPrimaryGetter();
    
    abstract public function getPrimaryObject();
        
    abstract public function getPrimaryObjectCollectionSetter();

    
    
    protected function generateMap(){
        for ($index = 0; $index < $this->list->size(); $index++) {
            /*@var $abstractMergerBean AbstractMergerBean*/
            $abstractMergerBean=$this->list->get($index);
            if(method_exists($abstractMergerBean,$this->getPrimaryGetter())){ //Checking if the method is present in the object
                $class=new ReflectionClass($abstractMergerBean); //fetcthing the actual id may required reflection in php7 
                $reflectionMethod=$class->getMethod($this->getPrimaryGetter()); //fetching the key for map
                $key=$reflectionMethod->invoke($abstractMergerBean); //key fetch
                if(!$this->map->contains($key)){//key is not present in exsisting map must be new key
                    $reflectionMethod2=$class->getMethod($this->getPrimaryObject()); //Must fetch primary object now. This can be Anything User, Profile
                    $primaryObject=$this->map->put($key,$reflectionMethod2->invoke($abstractMergerBean));
                }
                $primaryObject=$this->map->getValue($key);
                $this->logger->debug($primaryObject);
                if(method_exists($primaryObject,$this->getPrimaryObjectCollectionSetter())){
                    $this->logger->debug($abstractMergerBean->getActualResponseBean());
                    call_user_func_array(array($primaryObject,$this->getPrimaryObjectCollectionSetter()), array($abstractMergerBean->getActualResponseBean()));
                }else{
                    $this->logger->debug($this->getPrimaryObjectCollectionSetter() . "Method Not Present");
                }
            }
        }
    }
    
    public function toJson($isMapRequired = false) {
        $this->generateMap();
        $list=CollectionConverter::mapToSimpleList($this->map,true);
        $this->logger->debug($list);
        return ($isMapRequired)?$list:$list->toJson();
    }
    
}

?>
