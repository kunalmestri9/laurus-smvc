<?php
interface IFilterExceptionType{
    const INIT_FAILED=-29999;
    const AUTHENTICATION_FAILED=403;
    const TOKEN_EXPIRED=-20001;
    const INVALID_TOKEN=-20002;
    const INVALID_CREDENTIALS=-20003;
}
/**
 * Description of FilterException
 * Store the configuration for the Filter
 * @author kunal.mestri
 */
class FilterException extends Exception {
    
    public function __construct($message,$code=0) {
        parent::__construct($message);
        $this->code=$code;
    }
    
}

?>
