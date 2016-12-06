<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of SessionValidator
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.filters.IFilter");
class SessionValidator implements IFilter{
    protected $logger;
    protected $skipUrls;
    protected $redirectURL;
    protected $userLoginParam;
    protected $homePageUrl;
    public function getUserLoginParam() {
        return $this->userLoginParam;
    }

    public function setUserLoginParam($userLoginParam) {
        $this->userLoginParam = $userLoginParam;
    }

        public function __construct() {
        $this->logger= Logger::getLogger(__CLASS__);
    }
    public function doFilter(\HttpRequest $wR) {
        if(isset($_COOKIE["USER_COCK"])){
            if(TUtilString::contains($this->redirectURL,$wR->getURI())){
                 $wR->sendRedirect($this->homePageUrl);
                 return;
            }
            return;
        }
        $user=$wR->getSessionAttribute($this->getUserLoginParam(), true);
        if(!empty($user) && TUtilString::contains($this->redirectURL,$wR->getURI())){
            $wR->sendRedirect($this->homePageUrl);
            return;
        }
        if(count($this->skipUrls)){
            foreach ($this->skipUrls as $value) {
                $this->logger->debug("Checking Value :: ".$value ." :: with ::" .$wR->getURI());
                if(TUtilString::contains($wR->getURI() , $value)){
                    return ; //skipping filter if URL is skipping URL
                }
            }
        }
        if(empty($user)){
            $this->logger->debug("Accessing Forbidden page hence redirecting :".$this->redirectURL);
            header('Location: '.$this->redirectURL);
        }
       // header('Location: http://www.google.com/');
        
    }
    
    
     
    public function initFilter(\FilterConfig $f) {
        $this->userLoginParam=$f->getInitParameter("SESSION_USER_NAME");
        if(!TUtilString::isValidString($this->userLoginParam)){
            $this->userLoginParam="LOGIN_USER";
        }
        $this->skipUrls=$f->getInitParameter("skipUrls");
        $this->redirectURL=$f->getInitParameter("redirectUrl");
        $this->homePageUrl=$f->getInitParameter("homePageUrl");
        
    }    
}

?>
