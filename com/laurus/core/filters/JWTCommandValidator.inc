<?php
/**
 * Description of JWTCommandValidator
 * My job is make sure that commands that need to be guarded are check for JWT Implementation. 
 * everything else depends upon the worthy child for command specific securities.
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.filters.IFilter");
import("com.laurus.core.filters.IJWTCommandValidator");
importLib("php-jwt-master", "src/JWT.php");
importLib("php-jwt-master", "src/ExpiredException.php");
importLib("php-jwt-master", "src/BeforeValidException.php");
importLib("php-jwt-master", "src/SignatureInvalidException.php");
use \Firebase\JWT\JWT;

abstract class JWTCommandValidator implements IFilter , IJWTCommandValidator{
    protected $logger;
    protected $collectionManager;
    protected $userTypes;
    
    protected function stopUnauthorisedRequest(){
        header('Content-type: application/json');
        header('HTTP/1.0 401 Unauthorized');
        echo "{\"status\":\"error\",\"message\":\"unauthorized request\"}";
        die();
    }

    public function __construct() {
        $this->logger= Logger::getLogger(__CLASS__);
    }

    protected function getCommandName(){
        return isset ( $_REQUEST["command"]) ? $_REQUEST["command"] : false;  
    }

    protected function checkIfCommandPresentForValidation($commandName){
        return ($this->collectionManager) ? $this->collectionManager->getMethodForCommand($commandName) : false;
    }
    
    protected function objectToArray( $o ){
        $a = (array) $o;
        foreach( $a as &$v )
        {
            if( is_object( $v ) )
            {
                $v = $this->objectToArray( $v );
            }
        }
        return $a;
    }
    
    public function doFilter(\HttpRequest $wr) {
        $encapsulator=$this->checkIfCommandPresentForValidation($this->getCommandName());
        if( $encapsulator !== FALSE){
            // Now here I need to create plan for something extraorderinery for security. 
            $authHeader=$wr->getHeaderValue(strtolower("Authorization"));
            if(!$authHeader){
                $authHeader=$wr->getHeaderValue("Authorization");
            }
            if($authHeader){
                list($jwt) = sscanf($authHeader, 'Bearer %s');
                if($jwt){ // First make sure that every request can read jwt Token  --Checked
                    try{
                        $secretKey = base64_decode('APPCHEFUAMRUTA');
                        $token =$this->objectToArray(JWT::decode($jwt, $secretKey, array('HS512','HS256')));
                        $this->validateRequest($wr,$encapsulator,$token);
                        // Then check that token against the command requriment for security.
                        // Now this is upto the child
                        // Only then fire gun. 
                        // For this I need to cut this class legs make it abstract.
                     }catch(Exception $e){
                        $this->logger->error($e);
                        $this->stopUnauthorisedRequest();
                    }
                }else{ // Then make sure that JWT token is valid. --Checked
                    $this->stopUnauthorisedRequest();
                }
            }else{
                $this->stopUnauthorisedRequest();
            }
        }else{
            $this->logger->debug("validate request does not called");
        }
        

        //else just let them pass the wont do any harm
        /**
        * Only a worthy child can posses my powers.
        */
    }
    
    

    public function initFilter(\FilterConfig $f) {
        $collectionManagerName=$f->getInitParameter("collectionManager"); // This command are must to consider for sercurity
        if($collectionManagerName){
            $this->collectionManager=ClassLoader::loadName($collectionManagerName)->getInstance();
            $this->collectionManager->createCollection();
        }
    }    
}

?>
