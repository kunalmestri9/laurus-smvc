<?php
/**
 * Description of RefferalFilter
 * This filter will be mostly use to see the Refferal URLs .
 * E.g. In the case of LPanel you can specify this Refferal filter 
 * @author kunal.mestri
 */
class RefferalFilter implements IFilter {
    private $config;
    private $filterLessURL;
    public function doFilter(HttpRequest $wR) {
        if($this->manageException()){
            return;
        }
        //This means that the user is directly comming to the the page
        if($wR->getRefferer() != null){
            
        }
    }
    public function initFilter(FilterConfig $f) {
        $this->filterLessURL=$f->getInitParameter("exceptionUrls");
    }
    private function manageException(){
        foreach ($this->filterLessURL as $value) {
            if($wR->getURI()==$value){
                return true;
            }
        }
        return false;
    }
    private function curPageURL() {
        $pageURL = 'http';
        if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
        $pageURL .= "://";
        if ($_SERVER["SERVER_PORT"] != "80") {
         $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
        } else {
         $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
        }
        return $pageURL;
    }
}

?>
