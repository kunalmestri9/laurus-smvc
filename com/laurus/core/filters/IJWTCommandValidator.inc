<?php
/**
 * This will help to Declare the RULES associated with JWTCommandValidator
 * @author kunal.mestri
 */
import("com.laurus.core.filters.FilterConfig");
import("com.laurus.core.filters.FilterException");

interface IJWTCommandValidator {
    /**
     * throws InvalidRequestException
     * @param HttpRequest $wR
     */
    public function getUserTypes();
    public function getMethodToInvolkForCommand($commandName); //This can return the annonymous function or methor refference.
    public function validateRequest(\HttpRequest $http,$encapsulator,$token);    
}

?>
