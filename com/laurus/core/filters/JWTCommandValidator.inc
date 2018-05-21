<?php
/**
 * Description of JWTCommandValidator
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.filters.IFilter");
import("com.laurus.core.filters.IJWTCommandValidator");

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
        // Now here I need to create plan for something extraorderinery for security. 
        // First make sure that every request can read jwt Token 
        // Then make sure that JWT token is valid.
        // Then check that token against the command requriment for security.
        // Only then fire gun. 
        // For this I need to cut this class legs make it abstract.
        if($this->checkIfCommandPresentForValidation($this->getCommandName($wR)) !== FALSE){
            $this->validateRequest($wR);
        } 
        //else just let them pass the wont do any harm
    }
    
    

    public function initFilter(\FilterConfig $f) {
        $this->considerCommands=$f->getInitParameter("consider"); // This command are must to consider for sercurity
    }    
}

?>
