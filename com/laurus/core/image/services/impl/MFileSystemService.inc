<?php

import("com.laurus.core.image.services.api.IFileSystemService");
/**
 * Description of MFileSystemService
 *
 * @author kunal.mestri
 */
class MFileSystemService implements IFileSystemService {

    private $logger;
    private static $instance;
    private $filerSystemRootPath;

    public static function getInstance() {
        if (self::$instance == null) {
            self::$instance = new MFileSystemService();
        }
        return self::$instance;
    }

    private function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function createDirectory($path, $permission = 0777) {
        $path = $this->makePath($path);
        $this->logger->debug("This is full path " . $path);
        if (!file_exists($path)) {
            $this->logger->debug("inside if");
            $this->logger->debug("File Path is not present creating it");
            if (!mkdir($path, $permission, true)) {
                $this->logger->debug("Directory can not be created");
                throw new FileException(FileException_SUBTYPE::UNKNOWN, "Directory can not be created");
            }
        } else {
            $this->logger->error("Directory already exist.");
            throw new FileException(FileException_SUBTYPE::FILE_ALREADY_EXSISTS, "Directory can not be created");
        }
    }

    public function deleteDirectory($path, $permission = 0777) {
        $path = $this->makePath($path);
        if (is_dir($path) === true) {
            $files = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($path), RecursiveIteratorIterator::CHILD_FIRST);

            foreach ($files as $file) {
                if (in_array($file->getBasename(), array('.', '..')) !== true) {
                    if ($file->isDir() === true) {
                        rmdir($file->getPathName());
                    } else if (($file->isFile() === true) || ($file->isLink() === true)) {
                        unlink($file->getPathname());
                    }
                }
            }

            return rmdir($path);
        } else if ((is_file($path) === true) || (is_link($path) === true)) {
            return unlink($path);
        }

        return false;
    }

    public function createFile($path) {
        $path = $this->makePath($path);
        $handle = fopen($path, 'w');
        $this->logger->debug($path);
        if (!$handle) {
            $this->logger->debug("File " . $path . "  Can not be created Please try again");
            throw new FileException(FileException_SUBTYPE::UNKNOWN, "File Canot be created Please try again");
        }
        fclose($handle);
        return $path;
    }

    public function appendFile($path, $streamHandle, $length) {
        $path = $this->makePath($path);
        $handle = fopen($path, 'a');
        $this->logger->debug($path);
        if (!$handle) {
            $this->logger->error("File " . $path . "  Can not be open for appending Please try again");
            throw new FileException(FileException_SUBTYPE::UNKNOWN, "File Canot be created Please try again");
        }

        /* Read the data 1 KB at a time and write to the file */
        while ($data = fread($streamHandle, $length))
            fwrite($handle, $data);

        /* Close the streams */
        fclose($handle);
        fclose($streamHandle);
    }

    public function deleteFile($path) {
        $this->logger->debug("Deleting File " . $path);
        return unlink($path);
    }

    public function copyFile($sourcePath, $destinitionPath) {
        $this->logger->debug("Copying file from  " . $sourcePath . " to : " . $destinitionPath);
        return copy($sourcePath, $destinitionPath);
    }

    public function moveFile($sourcePath, $destinitionPath) {
        //TODO
    }

    public function init(\IServiceConfig $config) {
        $this->filerSystemRootPath = $config->getConfigParameter(IFileSystemService::ROOT_PATH_FOR_FILESYSTEM);
        $this->logger->debug($this->filerSystemRootPath);
    }

    public function makePath($path) {
        return $this->filerSystemRootPath . $path;
    }

    public function getFileExtension($fileName) {
        if ($fileName != NULL) {
            $fileExt = pathinfo($fileName);
            if ($fileExt['extension'] != null) {
                return "." . $fileExt['extension'];
            } else {
                $this->logger->error("Required File Extension");
                throw new ValidationException("Required File Extension");
            }
        } else {
            $this->logger->error("Please specified the file name..can't work with empty head");
            throw new ValidationException("Please specified the file name..can't work with empty head");
        }
    }

    public function fileExsistOrNot($path) {
        $path1 = $this->makePath($path);
        return file_exists($path1);
    }

    public function basicFileUpload($path) {
        $this->logger->debug($_FILES);
        print_r($_FILES); 
        $this->logger->debug("sdasdsadsadasdassad");
        if (move_uploaded_file($_FILES["file"]["tmp_name"], $path)) {

            // echo "The file " . basename($_FILES["fileToUpload"]["name"]) . " has been uploaded.";
        } else {
            throw new Exception("The file " . basename($_FILES["fileToUpload"]["name"]) . " has been not uploaded.", FileException_SUBTYPE::FILE_NOT_UPLOADED);
        }
    }

    public function copyDirectory($src, $dest) {
        chmod($src, 01777);
        if (!is_dir($dest)) {
            $oldumask = umask(0);
            mkdir($dest, 01777); // so you get the sticky bit set 
            umask($oldumask);
        }
        $dir_handle = @opendir($src) or die("Unable to open");
        while ($file = readdir($dir_handle)) {
            if ($file != "." && $file != ".." && !is_dir("$src/$file"))
                copy("$src/$file", "$dest/$file");
        }
        closedir($dir_handle);
    }

}

?>
