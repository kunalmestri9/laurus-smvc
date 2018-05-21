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
    protected $considerCommands;
    public function __construct() {
        $this->logger= Logger::getLogger(__CLASS__);
    }

    protected function getCommandName(){
        return isset ( $_REQUEST["command"]) ? $_REQUEST["command"] : false;  
    }

    protected function checkIfCommandPresentForValidation($commandName){
        return array_search($commandName,$this->considerCommands);
    }

    public function doFilter(\HttpRequest $wR) {
        if($this->checkIfCommandPresentForValidation($this->getCommandName($wR)) !== FALSE){
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
                        $token = JWT::decode($jwt, $secretKey, array('HS512'));
                        $this->validateRequest($wR,$token);
                        // Then check that token against the command requriment for security.
                        // Now this is upto the child
                        // Only then fire gun. 
                        // For this I need to cut this class legs make it abstract.
                     }catch(Exception $e){
                        $this->logger->debug($e);
                        header('HTTP/1.0 401 Unauthorized');
                        die(); //let me die I dont deserve to live;
                    }
                }else{ // Then make sure that JWT token is valid. --Checked
                    $this->logger->debug("Token missing.");
                    header('HTTP/1.0 401 Unauthorized');
                    die(); //let me die I dont deserve to live;
                }
            }else{
                $this->logger->debug("Token missing.");
                header('HTTP/1.0 401 Unauthorized');
                die(); //let me die I dont deserve to live
            }
        } 
        //else just let them pass the wont do any harm
        /**
         * Only a worthy child can posses my powers.
         */
    }
    
    

    public function initFilter(\FilterConfig $f) {
        $this->considerCommands=$f->getInitParameter("consider"); // This command are must to consider for sercurity
    }    
}

?>
