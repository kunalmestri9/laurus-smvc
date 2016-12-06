<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of SuccessResponse
 *
 * @author kunal.mestri
 */
import("com.laurus.core.json.jsonresp.DefaultJsonResponse");
class SuccessResponse extends DefaultJsonResponse {
    protected $object;
    public function toJson() {
       $str="{\"status\":\"success\"";
       if($this->message!=null){
            $str.=", \"message\" : \"".$this->message."\"" ;
       }  
       if($this->object!=null){
            $str.=", \"oresp\" : " . $this->object->toJson()  ;
       }if($this->messageType!=null){
            $str.=", \"messageType\" : \"" . $this->messageType."\""  ;
       }
       if($this->command!=null){
            $str.=", \"command\" :\"" . $this->command."\""  ;
       }         
       $str.="}";
       return $str;
    }    
    public function __construct($message=null,$object=null,$messageType=null,$command=null) {
        $this->message=$message;
        $this->object=$object;
        $this->messageType=$messageType;
        $this->command=$command;
    }
}

?>
