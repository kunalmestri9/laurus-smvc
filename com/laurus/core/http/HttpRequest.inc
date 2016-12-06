<?php
if (!function_exists('getallheaders')) { 
    function getallheaders() 
    { 
       $headers = ''; 
        foreach ($_SERVER as $name => $value) 
        { 
            if (substr($name, 0, 5) == 'HTTP_') 
            { 
                $headers[str_replace(' ', '-', strtolower(str_replace('_', ' ', substr($name, 5))))] = $value; 
            } 
        } 
         return $headers; 
    } 
}
/**
 * Description of HttpRequest
 * This method will wrap the $_REQUEST to provide better APIs on Request object
 * @author kunal.mestri
 */
import("com.laurus.core.http.HttpSession");
class HttpRequest{
    /**
     *  GetParameter
     * @param type $str
     * @return String
     * @desc This method will return either String or String[]
     */
    public /* String or String[]*/ function getParameter($str){
        if($str=="")return null;
        return isset($_REQUEST[$str])?$_REQUEST[$str]:null ;
    }
    /**
     *  SetParameter
     * @param type $str
     * @return void
     * @desc This method will return either String or String[]
     */
    public function setParameter($key,$val){
        if($key=="" || $val=="")return null;
        $_REQUEST[$key]=$val;
    }
    /**
     * GetParameterNames
     * @return String[]
     * @desc All parameters that are in the part of the request.
     */
    public /* String[] */ function getParameterNames(){
        return array_keys($_REQUEST);
    }
    /**
     * GetHeaders
     * @return  String[]
     */
    public /* String[] */ function getHeaders(){
        return getallheaders();
    }
    /**
     * 
     * @param String $headerName
     * @return String
     */
    public /* String */ function getHeaderValue(/*String*/ $headerName){
         
        $headers=getallheaders();
        return ($headers[$headerName]!=null)?$headers[$headerName]:null ;
    }
    /**
     * 
     * @return \HttpSession
     */
    public /*HttpSession*/ function getSession(){
       return new HttpSession();
    }
    /**
     * getRemoteAddress is used to retrive the IP of the user 
     * @return String
     */
    public /*String*/ function getRemoteAddress(){
        return $_SERVER["REMOTE_ADDR"];
    }
    
    public /*String*/ function getUserAgent(){
        return $_SERVER["HTTP_USER_AGENT"];
    }
    
    public /*String*/ function getRefferer(){
        return $_SERVER["HTTP_REFERER"];
    }
    public function getURI(){
        $v=parse_url($_SERVER["REQUEST_URI"]);
        return $v['path'];
    }
    
    public function getCommandAttribute($attributeName,$unserialized=false){
        if($attributeName=="")return null;
        if($unserialized)
            return isset($_REQUEST[$_REQUEST['command']][$attributeName])? unserialize($_REQUEST[$_REQUEST['command']][$attributeName]):null ;
        else
            return isset($_REQUEST[$_REQUEST['command']][$attributeName])?$_REQUEST[$_REQUEST['command']][$attributeName]:null ;
    }
    
    public function getSessionAttribute($attributeName,$unserialized=false){
        if($attributeName=="")return null;
        if($unserialized)
            return isset($_SESSION[$attributeName])? unserialize($_SESSION[$attributeName]):null ;
        else
            return isset($_SESSION[$attributeName])? $_SESSION[$attributeName]:null ;
    }
    
    public function setSessionAttribute($attributeName,$obj){
        if($attributeName=="")return false;
        $_SESSION[$attributeName]=serialize($obj);
        return true;
    }
    public function removeSessionAttribute($attributeName){
        if($attributeName=="")return false;
                unset($_SESSION[$attributeName]);
        return true;
    }
    
    public function setCommandAttribute($attributeName,$value){
        $_REQUEST[$_REQUEST['command']][$attributeName]=$value;
    }
    
    public function setJSONResponseBean($object){
        $_SESSION["JSON_RESPONSE_BEAN"]=$object;
    }
    
    public function sendRedirect($url){
        header('Location: ' . $url);
        die();
    }
    
    public function getUserFromSessionOrRequest($key="LOGIN_USER"){
        $user=$this->getSessionAttribute($key,true);//retrive it from the session for Web APIs
        if(!$user){
             $user=$this->getCommandAttribute("validatedUser",true);//retrive it from the command attribute for Mobile APIs
             if(!$user)
                throw new ValidationException("User does not exist in session or request",isset($_SERVER["LSESSION_OUT"])?$_SERVER["LSESSION_OUT"]:80000);
        }
        return $user;
    }
}

?>
