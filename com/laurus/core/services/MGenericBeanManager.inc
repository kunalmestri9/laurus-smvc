<?php
import("com.laurus.core.services.MDBService");
import("com.laurus.core.services.IGenericBeanManager");
/**
 * Description of IGenericBeanManager
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class MGenericBeanManager  extends MDBService implements IGenericBeanManager {
    private static $instance;
        
    public function addBeanToDB($bean) {
        $this->addToDb($bean);
        return $bean;
    }
    
    public function updateBeanToDb($bean) {
        $this->updateToDb($bean);
        return $bean;
    }

    public function init(\IServiceConfig $config) {
        
    }

    public static function getInstance() {
        if(self::$instance==null){
            self::$instance=new MGenericBeanManager();
        }
        return self::$instance;
    }

    public function getObjectByBean($bean,$isComposite=false) {
        $this->findById($bean,$isComposite);
        return $bean;
    }

    public function getCollectionByBean($bean,$isComposite=false) {
        $simpleList=$this->findCollectionById($bean,$isComposite);
        return $simpleList;
    }
     public function getCustomizedQueryBean($bean,\Statement $statement,$isComposite=false) {
        $simpleList=$this->getByCustomQuery($bean,$statement->getQuery(),$isComposite);
        return $simpleList;
    }

    public function addCollectionToDB(\SimpleList $simpleList) {
        for ($index = 0; $index < $simpleList->size(); $index++) {
            $bean=$simpleList->get($index);
            $this->addBeanToDB($bean);
        }
        return $simpleList;
    }

    public function deleteObjectBean($bean) {
        $this->deleteFromDb($bean);
    }

}
