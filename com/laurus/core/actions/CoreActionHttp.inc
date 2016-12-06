<?php
import("com.laurus.core.actions.IAction");
import("com.laurus.core.actions.IHttpAction");

/**
 * Description of CoreActionHttp
 *
 * @author kunal.mestri
 */
abstract class CoreActionHttp implements IAction , IHttpAction{
    protected $http;
    public function __construct() {
        $this->http=new HttpRequest();
    }
    public function invoke() {
        if($this->http==null){
            $this->http=new HttpRequest();
        }
        return $this->executeInvoke($this->http);
    }
    protected function validateRequestParameters(HttpRequest $wr ,$mandatoryparams){
        foreach ($mandatoryparams as $key => $value) {
            if(!TUtilString::isValidString($wr->getParameter($key)))
                throw new ValidationException($value ."  is mandatory");
        }
    }
    
    protected function validateBean($bean,$mandatoryparams,$code=20000){
        $reflectionClass=new ReflectionClass(get_class($bean));
        foreach ($mandatoryparams as $key => $value) {
            $methodName="get".ucfirst($key);
            if($reflectionClass->hasMethod($methodName)){
                /*@var $refMethod ReflectionMethod*/
                $refMethod=$reflectionClass->getMethod($methodName);
                $val=$refMethod->invoke($bean);
                if(!TUtilString::isValidString($val)){
                    throw new ValidationException($value ."  is mandatory",$code);
                }
            }
        }
    }
}

?>
