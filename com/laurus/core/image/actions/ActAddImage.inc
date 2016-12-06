<?php
import("com.laurus.core.image.actions.ActImageAction");

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of ActAddImage
 *
 * @author Amruta
 */
class ActAddImage extends ActImageAction{
    protected $logger;
      
    public function  __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }

   
    public function imageInvoke(\IImageService $imageservice, \HttpRequest $wr) {
                $this->logger->debug("Inside " . __CLASS__ ." ".__METHOD__);
        $this->validateRequestParameters($wr, array(
            "image_name" => "Image Name"
        ));
        
        $image=new Image();
        $image->setImageName($wr->getParameter('image_name'));
        try {
            $newimage=$imageservice->addImage($image);
        }catch (\SqlException $exc) {
                $this->logger->error($exc->getMessage());
                throw new ValidationException("Image Can not added", IImageExceptionType::UNKNOWN);
        }catch(Exception $e){
            throw new ValidationException("Please try again later",  IImageExceptionType::UNKNOWN);
        }
        $_REQUEST["SUCCESS_MESSAGE"]="Image added successfully";
        $wr->setParameter("imageId", $newimage->getId());
        $wr->setJSONResponseBean($newimage);
        return $newimage;
    }
   
}

?>
