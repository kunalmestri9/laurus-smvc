<?php
import("com.laurus.core.image.actions.IImageAction");
import("com.laurus.core.actions.CoreActionHttp");
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of ActImageAction
 *
 * @author Shripad
 */
abstract class ActImageAction extends CoreActionHttp implements IImageAction {
    protected $logger;
      
    public function  __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }
    
    public function executeInvoke(\HttpRequest $wr) {
        $this->logger->debug("in execute invoke");
        /* @var $mImageServiceobj IImageService */
        $mImageServiceobj=ServiceManager::getInstance()->getService(IImageService::NAME);
        return $this->imageInvoke($mImageServiceobj,$wr);
    }
    
    protected function doSimpleJsonConversions($json,$class,$isFullName=false){
        $jsonObjectConverter=new JsonToObjectConverter();
        if($isFullName===false)
            $bean=new $class; //It is expected that you have loaded all the entities bellow this line if not import them into service interface
        $object=$jsonObjectConverter->convertToObject($json,(($isFullName===false)?$bean:$class));
        return $object;
    }
    
    protected function doSimpleJsonSimplelistConversions($json,$class){
        $jsonObjectConverter=new JsonToObjectConverter();
        $bean=new $class; //It is expected that you have loaded all the entities bellow this line if not import them into service interface
        $collection=$jsonObjectConverter->convertSimpleListOfObjects($json,$bean);
        return $collection;
    }

}

?>
