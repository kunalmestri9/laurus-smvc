<?php
/**
 * Description of SuccessResponse
 *
 * @author kunal.mestri
 */
import("com.laurus.core.json.jsonresp.DefaultJsonResponse");
class ErrorResponse extends DefaultJsonResponse {
    private $code;
    public function toJson() {
       $str="{\"status\":\"error\"";
       if($this->message!=null){
            $str.=", \"message\" : \"".$this->message."\"" ;
       } 
       if($this->code!=null){
            $str.=", \"code\" : \"".$this->code."\"" ;
       }
       if($this->command!=null){
            $str.=", \"command\" : " . $this->command  ;
       }
       $str.="}";
       return $str;
    } 
    
    public function getCode() {
        return $this->code;
    }

    public function setCode($code) {
        $this->code = $code;
    }
    public function __construct($message=null,$command=null) {
        $this->message=$message;
        $this->command=$command;
    }

    
}

?>
