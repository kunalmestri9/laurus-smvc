<?php
import("com.laurus.core.actions.CoreActionHttp");
import("com.laurus.core.image.services.api.IFileSystemService");
import("com.laurus.core.image.services.api.IImageService");

/**
 * Description of ActFileUpload
 *
 * @author Kunal
 */
class ActFileUpload extends CoreActionHttp{
    protected $logger;
      
    public function  __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }
    public function executeInvoke(\HttpRequest $wr) {
        $this->logger->debug("reached here");
        $this->logger->debug($wr->getHeaders());
        $fileId=base64_decode($wr->getHeaderValue("path"));
        $fileType=$wr->getHeaderValue("filetype");
        if(!TUtilString::isValidString($fileType)){
            $fileType=base64_decode($wr->getHeaderValue("fileType"));
        }
        $length=$wr->getHeaderValue("Content-Length");
        if($length==null){
            $length=$wr->getHeaderValue("laurus-content-length");
        }
        $streamHandle= fopen("php://input", "r");
        if(TUtilString::equals("file", $fileType)){
//            $this->logger->debug("ethe allo");
//             $this->logger->debug($fileId);
//            /*@var $mFileDbService IFileDbService*/
//            $mFileDbService=ServiceManager::getInstance()->getService(IFileDbService::NAME);
//             
//            $file=$mFileDbService->getFileFromId($fileId);
//            $filePath=base64_decode($file->getFilePath());
//            $this->logger->debug($file);
        }
        else if(TUtilString::equals("image", $fileType)){
            $this->logger->debug("file Id");
            $this->logger->debug($fileId);
            /*@var $mImageService IImageService*/
            $mImageService=ServiceManager::getInstance()->getService(IImageService::NAME);
            $image=$mImageService->getImageFromId($fileId);
            $newPath=$image->getImagePath();
            $pieces=  explode("/", $newPath);
            $filePath=base64_decode(end($pieces));
            
        }
        /*@var $mFileSystemService IFileSystemService*/
        $mFileSystemService=ServiceManager::getInstance()->getService(IFileSystemService::NAME);
        $this->logger->debug("file path from headers");
        $this->logger->debug($filePath);
        $mFileSystemService->appendFile($filePath, $streamHandle, $length);
    }
}

?>
