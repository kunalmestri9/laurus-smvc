<?php
import("com.laurus.core.actions.IAction");
import('com.laurus.core.thumbnail.IThumbnailGenerator');
import("com.laurus.core.image.services.api.IFileSystemService");
class ActImageCompresser Implements IAction {
    private $logger;
    public function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function invoke() {
        $wR=new HttpRequest();
        $rootPath=$_SERVER["REOPENLIMA_PATH"];
        $path=base64_decode($wR->getParameter("file"));
        $this->logger->debug("Woow");
        $this->logger->debug(base64_decode($path));
        //$newPath=str_replace("/","\\",$path);
        $filePath=$rootPath.$path;
//        $DS=($_SERVER['WORKSPACE_OS']!="LINUX")?"\\":"/";
        $filename=explode(DIRECTORY_SEPARATOR, $filePath);
        $file=end($filename);
        $ext1=explode(".",$file);
        $ext=end($ext1);
        $mimes = array(
            'jpg' => 'image/jpg',
            'jpeg' => 'image/jpg',
            'gif' => 'image/gif',
            'png' => 'image/png'
        );
         
        $type=$mimes[strtolower($ext)];
        $serviceProvider=ServiceManager::getInstance();
        $this->logger->debug($path);
        $this->logger->debug($type);
        /*@var $imageThumbnailGenerator IThumbnailGenerator*/
        $imageThumbnailGenerator=$serviceProvider->getService(IThumbnailGenerator::NAME);
        try{
            
            if(TUtilString::equals("imageCompressD",$wR->getParameter("command"))){
                $mImageServiceRes=$imageThumbnailGenerator->generateDynamic($filePath,$type,$wR->getParameter("l"));
            }else{
                if($imageThumbnailGenerator->doesNeedToBeCreate($filePath))
                    $mImageServiceRes=$imageThumbnailGenerator->generate($filePath,$type);
            }
            
        }  catch (Exception $e){
            $this->logger->error("error while fetching image ".$path);   
            throw new ValidationException("error while fetching Image");
        }
         $_REQUEST["SUCCESS_MESSAGE"]="Image Compress successfully";
            
        
        
    }
}

?>
