<?php

/**
 * Description of SessionValidatorApi
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.filters.IFilter");
import("com.laurus.core.exception.ValidationException");
import("com.laurus.core.filters.IFilterHelper");

class SessionValidatorApi implements IFilter{
    private $logger;
    private $skipCommands;
    private $userLoginParam;
    private $tokenKey;
    private $authenticationServiceKey;
    private $userKey;
    private $helperClass;
    private $skipUrlSecKey;

    
    public function getHelperClass() {
        return $this->helperClass;
    }

    public function setHelperClass($helperClass) {
        $this->helperClass = $helperClass;
    }
    
    public function getAuthenticationServiceKey() {
        return $this->authenticationServiceKey;
    }

    public function setAuthenticationServiceKey($authenticationServiceKey) {
        $this->authenticationServiceKey = $authenticationServiceKey;
    }

        
    public function getUserKey() {
        return $this->userKey;
    }

        
    public function getTokenKey() {
        return $this->tokenKey;
    }

    
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
        $user=$wR->getSessionAttribute($this->getUserLoginParam(), true);
		$this->logger->debug( $_SERVER);
        if(TUtilString::equals("OPTIONS" , $_SERVER['REQUEST_METHOD'])){
            $this->logger->debug("insdjhsdfjsd");
            header("Access-Control-Allow-Headers: origin, content-type, accept,seck,uid");
            die();
        }
        if(count($this->skipCommands)){
            foreach ($this->skipCommands as $value) {
                $this->logger->debug("Checking Value :: ".$value ." :: with ::" .$wR->getParameter("command") );
                if(TUtilString::equals($wR->getParameter("command") , $value)){
                    return ; //skipping filter if URL is skipping URL
                }
            }
        }
        if(count($this->skipUrlSecKey)){
            foreach ($this->skipUrlSecKey as $value) {
                $this->logger->debug("Checking Value :: ".$value ." :: with ::" . $wR->getHeaderValue($this->getTokenKey()));
                if(TUtilString::equals($wR->getHeaderValue($this->getTokenKey()) , $value)){
                    return ; //skipping filter if URL is skipping URL
                }
            }
        }
        if(empty($user)){
            $helper=ClassLoader::loadName($this->getHelperClass())->getInstance();
            if($helper instanceof IFilterHelper){
                $helper->validateSecurityToken($wR,$this);
            }
        }
    }
    
     
    public function initFilter(\FilterConfig $f) {
        $this->userLoginParam=$f->getInitParameter("SESSION_USER_NAME");
        $this->tokenKey=$f->getInitParameter("TOKEN_SECURITY_HEADER_KEY");
        $this->authenticationServiceKey=$f->getInitParameter("VALIDATION_SERVICE_NAME");
        $this->userKey=$f->getInitParameter("TOKEN_SECURITY_USER_ID");
        $this->helperClass=$f->getInitParameter("helper");
        
        if(!TUtilString::isValidString($this->userLoginParam)){
            $this->userLoginParam="LOGIN_USER";
        }
        $this->skipCommands=$f->getInitParameter("skipCommands");
        $this->skipUrlSecKey=$f->getInitParameter("skipUrlSeckKey");
    }    
}

?>
