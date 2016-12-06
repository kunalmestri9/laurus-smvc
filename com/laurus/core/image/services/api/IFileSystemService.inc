<?php
import("com.laurus.core.services.IService");
interface IFileSystemService extends IService{
   const NAME="FILESYSTEM_SERVICE";
   const ROOT_PATH_FOR_FILESYSTEM="FILESYSTEM_ROOT_PATH";
   public function createDirectory($path,$permission=0777);
   public function createFile($path);
   public function appendFile($path,$data,$length);
   public function  deleteFile($path);
   public function  copyFile($sourcePath,$destinitionPath);
   public function  moveFile($sourcePath,$destinitionPath);
   public function getFileExtension($fileName);
   public function fileExsistOrNot($path);
   public function deleteDirectory($path,$permission=0777);
   public function basicFileUpload($path);
   public function copyDirectory($src,$dest);
}
?>
