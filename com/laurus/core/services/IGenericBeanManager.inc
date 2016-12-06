<?php
import("com.laurus.core.services.IService");
/**
 * Description of IGenericBeanManager
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
interface IGenericBeanManager extends IService {
    const NAME="GENERIC_DB";
    public function addBeanToDB($bean);
    public function addCollectionToDB(\SimpleList $simpleList);
    
    public function updateBeanToDb($bean);
    public function getObjectByBean($bean,$isComposite=false);
    public function getCollectionByBean($bean,$isComposite=false);
    public function getCustomizedQueryBean($bean,\Statement $stmt,$isComposite=false);
    public function deleteObjectBean($bean);
    
}
