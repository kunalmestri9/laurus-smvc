<?php
import("com.laurus.core.coreent.LCoreBean");
/**
 * Description of Image
 *
 * @author ABC
 */
class Image {

    private $id;
    private $imageName;
    private $createdDate;
    private $imagePath;
    private $desc;
    private $dPath;
    public function getDPath() {
        return $this->dPath;
    }

    public function setDPath($dPath) {
        $this->dPath = $dPath;
    }

    public function getDesc() {
        return $this->desc;
    }

    public function setDesc($desc) {
        $this->desc = $desc;
    }

    public function getId() {
        return $this->id;
    }

    public function setId($id) {
        $this->id = $id;
    }

    public function getImageName() {
        return $this->imageName;
    }

    public function setImageName($imageName) {
        $this->imageName = $imageName;
    }

    public function getCreatedDate() {
        return $this->createdDate;
    }

    public function setCreatedDate($createdDate) {
        $this->createdDate = $createdDate;
    }

    public function getImagePath() {
        return $this->imagePath;
    }

    public function setImagePath($imagePath) {
        $this->imagePath = $imagePath;
    }

    public function toJson() {
        $map = new Map();
        $map->put("id", $this->getId());
        $map->put("imageName", $this->getImageName());
        $map->put("createdDate", $this->getCreatedDate());
        $map->put("desc", $this->getDesc());
        $map->put("dpath",$this->getDPath());
        if ($this->getImagePath() != "") {
            $imagePathArray = explode("/", $this->getImagePath());
            if (TUtilString::equals("org", end($imagePathArray))) {
                $map->put("imagePath", $this->getImagePath());
                $map->put("thumb1", rtrim($this->getImagePath(),"/org") . "/tn");
                $map->put("thumb2", rtrim($this->getImagePath(),"/org") . "/tm");
                $map->put("thumb3", rtrim($this->getImagePath(),"/org") . "/ts");
                $map->put("thumb4", rtrim($this->getImagePath(),"/org") . "/ev");
                $map->put("thumb5", rtrim($this->getImagePath(),"/org") . "/cd");
            }else{
                $map->put("imagePath", $this->getImagePath() . "/org");
                $map->put("thumb1",  $this->getImagePath(). "/tn");
                $map->put("thumb2", $this->getImagePath() . "/tm");
                $map->put("thumb3", $this->getImagePath(). "/ts");
                $map->put("thumb4", $this->getImagePath() . "/ev");
                $map->put("thumb5", $this->getImagePath() . "/cd");
            }

        }

        return $map->toJson();
    }

}

?>
