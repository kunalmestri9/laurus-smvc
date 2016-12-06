<?php
import("com.laurus.core.services.IService");
import("com.laurus.core.image.ent.*");
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of IImageService
 *
 * @author Amruta
 */
interface IImageService  extends IService{
    const NAME="IMAGE_SERVICE";
    public function addImage(\Image $image);
    public function updateImageDirectoryPathInDb(\Image $image); 
    public function getImageFromId($imageId); 
    public function copyImageToDestination($source,$destination);
}

?>
