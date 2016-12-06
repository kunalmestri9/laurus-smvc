<?php

import("com.laurus.core.image.actions.ActImageAction");
/**
 * Description of ActMakeTempImagePath
 *
 * @author AMRUTA
 */
class ActMakeTempImagePath extends ActImageAction{
    protected $logger;

    public function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function imageInvoke(\IImageService $imageservice, \HttpRequest $wr) {
       
        /* @var $image Image */
        $image = $wr->getCommandAttribute("aimage", true);
        $this->logger->debug($image);
        /* @var $mFileServiceobj IFileSystemService */
        $mFileServiceobj = ServiceManager::getInstance()->getService(IFileSystemService::NAME);
        $path = 'tmp'.DIRECTORY_SEPARATOR.'images'.DIRECTORY_SEPARATOR ;
        $newPath = $path . $image->getId() . $mFileServiceobj->getFileExtension($image->getImageName());
        $image->setImagePath(base64_encode($newPath));
        if (!$mFileServiceobj->fileExsistOrNot($path)) {
            $mFileServiceobj->createDirectory($path);
        } else {
            
        }
        $fullpath = $mFileServiceobj->createFile($newPath);
        $header = new Map();
        $this->logger->debug(base64_encode($newPath));
        $header->put("path", base64_encode($image->getId()));
        $header->put("filetype", base64_encode("file"));
        $_REQUEST["HEADER_CONTAINER"] = $header;
        $wr->setParameter("path", $fullpath);
        $wr->setJSONResponseBean($image);
        $fileList = new SimpleList();
        $fileList->add($image);
        return $fileList;
    }
}

?>
