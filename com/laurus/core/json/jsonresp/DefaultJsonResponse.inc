<?php
/**
 * Description of DefailJsonResponse
 *
 * @author kunal.mestri
 */
abstract class DefaultJsonResponse {
    protected $message;
    
    abstract public function toJson();
    public function __construct($message=null) {
        $this->message=$message;
    }
}

?>
