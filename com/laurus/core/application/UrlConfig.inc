<?php
/**
 * Description of UrlConfig
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class UrlConfig {
    /**
     *
     * @var string
     */
    private $url;
    /**
     *
     * @var string 
     */
    private $template;
    /**
     *
     * @var Map 
     */
    private $regExMap;
    
    /**
     *
     * @var Map 
     */
    private $customRegExType;
    /**
     *
     * @var string 
     */
    private $command;
    
    
    private $isAbsolute;
    
    
    public function getIsAbsolute() {
        return $this->isAbsolute;
    }

    public function setIsAbsolute($isAbsolute) {
        $this->isAbsolute = $isAbsolute;
    }

        
    public function getCommand() {
        return $this->command;
    }

    public function setCommand($command) {
        $this->command = $command;
    }

        
    public function __construct($url,$tmpl) {
        $this->url=$url;
        $this->template=$tmpl;
        $this->isAbsolute=false;
    }


    public function getRegExMap() {
        return $this->regExMap;
    }

    public function setRegExMap(Map $regExMap) {
        $this->regExMap = $regExMap;
    }
    
    public function getUrl() {
        return $this->url;
    }

    public function setUrl($url) {
        $this->url = $url;
    }

    public function getTemplate() {
        return $this->template;
    }

    public function setTemplate($template) {
        $this->template = $template;
    }

    public function getCustomRegExType() {
        return $this->customRegExType;
    }

    public function setCustomRegExType(Map $customRegExType) {
        $this->customRegExType = $customRegExType;
    }
    
    public function isCommandPresent(){
        return TUtilString::isValidString($this->command);
    }
}

?>
