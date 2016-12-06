<?php
import("com.laurus.core.json.jsonresp.ErrorResponse");
import("com.laurus.core.json.jsonresp.SuccessResponse");
/**
 * Description of JsonObjectWriter
 *
 * @author kunal.mestri
 */
class JsonObjectWriter {
    
    public function __construct() {
        
    }
    public function renderJson(){
        $jsonBean=$_SESSION["JSON_RESPONSE_BEAN"];
        if($_REQUEST["ERROR_CONTAINER"]!=null){
            $jsonBean=new ErrorResponse($_REQUEST["ERROR_CONTAINER"]);
            $jsonBean->setCode($_REQUEST["ERROR_CODE"]);
        }
        if($jsonBean==null){
            //Assign default success response
            $jsonBean=new SuccessResponse($_REQUEST["SUCCESS_MESSAGE"]);
        }
        if(isset($_REQUEST["HEADER_CONTAINER"]) && $_REQUEST["HEADER_CONTAINER"] instanceof Map){
            /* @var $responseHeaders Map*/
            $responseHeaders=$_REQUEST["HEADER_CONTAINER"];
            $iterator=$responseHeaders->getIterator();
            while($iterator->hasNext()){
                /* @var $ent Entry */
                $ent=$iterator->next();
                header($ent->getKey().":".$ent->getValue()) ;
            }
        }
        //var_dump($jsonBean);echo "<br/>kunal";
        if($jsonBean!=null){
            header('Content-type: application/json');
            if(!($jsonBean instanceof ErrorResponse)  && !($jsonBean instanceof SuccessResponse) ){
                $newBean=new SuccessResponse($_REQUEST["SUCCESS_MESSAGE"],$jsonBean);
                echo $newBean->toJson();
            }else{
                echo $jsonBean->toJson();
            }
            
        }
    }
}

?>
