<?php

import("com.laurus.core.image.services.api.IImageService");
import("com.laurus.core.connection.CoreConnection");
import("com.laurus.core.image.services.api.IFileSystemService");
import("com.laurus.core.json.JsonToObjectConverter");
import("com.laurus.core.services.TQueryList");
import("com.laurus.core.collection.SimpleList");



/**
 * Description of MImageService
 *
 * @author Amruta
 */
class MImageService implements IImageService {

    /**
     *
     * @var <MImageService>
     */
    private static $instance;
    private static $SERVICE_NAME = "image";

    /**
     *
     * @var <CoreConnection>
     */
    private $conn;

    /**
     *
     * @var <Logger>
     */
    private $logger;

    /**
     * Giving Singleton behaviour
     * @return <MImageService>
     */
    public function init(\IServiceConfig $config) {
        
    }

    private function __construct() {
        $this->conn = new CoreConnection();
        $this->conn->setConnectionType(IConnectionType::SIMPLECONNECTION);
        $this->logger = Logger::getLogger(__CLASS__);
        $this->mFileSystemService = ServiceManager::getInstance()->getService(IFileSystemService::NAME);
    }

    public static function getInstance() {
        if (self::$instance == null) {
            self::$instance = new MImageService();
        }
        return self::$instance;
    }

    public function addImage(\Image $image) {
        $this->logger->debug("inside addphotoToImage");
        $createdDate = date("Y-m-d H:i:s");
        $query = TQueryList::getInstance()->getQuery("addImage", self::$SERVICE_NAME);
        $stmt = $this->conn->createStatement();
        $stmt->setQuery($query);
        $stmt->setString(1, $image->getImageName());
        $stmt->setString(2, $createdDate);
        $imageId = $this->conn->executeUpdate($stmt, true);
        $image->setId($imageId);
        $image->setCreatedDate($createdDate);
        return $image;
    }

     public function updateImageDirectoryPathInDb(\Image $image) {
        $this->logger->debug(__METHOD__);
        $query = TQueryList::getInstance()->getQuery("addimagePathInDb", self::$SERVICE_NAME);
        $stmt =  $this->conn->createStatement();
        $stmt->setQuery($query);
        $stmt->setString(1, $image->getImagePath());
        $stmt->setString(2, $image->getId());
        $this->conn->executeUpdate($stmt,true);
    }

    
    public function getImageFromId($imageId) {
        $this->logger->debug(__METHOD__);
        $query = TQueryList::getInstance()->getQuery("getImageFromId", self::$SERVICE_NAME);
        $stmt =  $this->conn->createStatement();
        $stmt->setQuery($query);
        $stmt->setInt(1, $imageId);
        $res=$this->conn->execute($stmt);
        $this->logger->debug($stmt->getQuery());
        return $this->translateImage($res,true);

    }
    
     public static function translateImage(\Resultset $res, $imagePath = false) {
        $image = null;
        while ($res->next()) {
            $image = new Image();
            $image->setId($res->get("IMAGE_ID"));
            $image->setImageName($res->get("IMAGE_NAME"));
            $image->setCreatedDate($res->get("CREATED_DATE"));
            $image->setImageType($res->get("IMAGE_TYPE"));
            $image->setImagePath($_SERVER['V_PATH'] . "content/" . base64_encode($res->get("IMAGE_ID")));
            $image->setDPath($_SERVER['V_PATH'] . "downloadimg/" . base64_encode($res->get("IMAGE_ID")));
            if ($imagePath) {
                $image->setImagePath($res->get("IMAGE_PATH"));
            }
        }
        return $image;
    }
    
    public function copyImageToDestination($source,$destination){
      
        $encodedDestinationPath=base64_decode($destination);
        $encodedSourcePath=base64_decode($source);
        $this->logger->debug($encodedSourcePath);
        $folderPath=substr($encodedDestinationPath,0,strrpos($encodedDestinationPath,DIRECTORY_SEPARATOR));
        $this->logger->debug($folderPath);
        if (!$this->mFileSystemService->fileExsistOrNot($folderPath)) {
            $this->mFileSystemService->createDirectory($folderPath);
        } else {
            
        }
        $this->logger->debug($this->mFileSystemService->makePath(base64_decode($source)));
        $this->logger->debug($this->mFileSystemService->makePath(base64_decode($destination)));
        
        //move_uploaded_file($this->mFileSystemService->makePath($encodedSourcePath),$this->mFileSystemService->makePath($encodedDestinationPath));
       $this->mFileSystemService->copyfile($this->mFileSystemService->makePath($encodedSourcePath),$this->mFileSystemService->makePath($encodedDestinationPath));
       $this->logger->debug('copy hogaya');
       return;
    }
}

?>
