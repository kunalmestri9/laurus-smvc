<?php

import("com.laurus.core.image.actions.ActImageAction");
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of ActGetImage
 *
 * @author ABC
 */
class ActGetImage extends ActImageAction {

    protected $logger;

    public function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function imageInvoke(\IImageService $imageService, \HttpRequest $wr) {
        $this->logger->debug(__METHOD__);
        $requestImageId = $wr->getParameter("imid");
        $imageId = base64_decode($requestImageId);
        if (TUtilString::isValidString($requestImageId)) {
            $this->logger->debug($imageId);
            $image = $imageService->getImageFromId($imageId);
            if ($image) {
                $newPath = $image->getImagePath();
                $this->logger->debug($newPath);
                $pieces = explode("/", $newPath);
                $filePath = end($pieces);
                $this->logger->debug($filePath);
                $wr->setParameter("file", $filePath);
                $wr->setParameter("filename", $image->getImageName());
            }
        }
    }

//put your code here
}

?>
