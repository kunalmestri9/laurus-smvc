<?php
/**
 * Description of FilterExecutor
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
import("com.laurus.core.filters.IFilterExecutor");
class FilterExecutor implements IFilterExecutor {
    private $filterConfig;
    private $logger;
    public function __construct() {
        $this->logger=  Logger::getLogger(__CLASS__);
    }
    public function executeFilters() {
        foreach ($this->filterConfig as $key =>$value) {
            $this->logger->debug("Loading filter : " . $key );
            if(!isset($value["class"])){
                $this->logger->error($key ." filter class name is missing");
            }
            $filterObject=ClassLoader::loadName($value["class"])->getInstance();
            try{
                $filterObject->initFilter(new FilterConfig($value["config"]));
                $filterObject->doFilter(new HttpRequest());
            }catch (FilterException $fe){
                 if($fe->getCode()==IFilterExceptionType::INIT_FAILED){
                     $this->logger->error(" Filter init failed");
                     $this->throwHttpError(500);
                 }
                 if($fe->getCode()==IFilterExceptionType::INVALID_TOKEN){
                     $this->logger->error(" Filter Invalid token");
                     $this->throwHttpError(IFilterExceptionType::AUTHENTICATION_FAILED);
                 }
                 if($fe->getCode()==IFilterExceptionType::TOKEN_EXPIRED){
                     $this->logger->error("Filter token expired");
                     $this->throwHttpError(IFilterExceptionType::AUTHENTICATION_FAILED);
                 }
                 if($fe->getCode()==IFilterExceptionType::AUTHENTICATION_FAILED){
                     $this->logger->error("Filter token expired");
                     $this->throwHttpError(IFilterExceptionType::AUTHENTICATION_FAILED);
                 }
                 $this->throwHttpError($fe->getCode());
            }catch (Exception $e){
                 $this->logger->error($key ." Filter init failed");
                 $this->logger->error($e);
                 $this->throwHttpError(500);
            }
        }
    }

    public function setFiltersConfiguration($config) {
        $this->filterConfig=$config;
    }   
    
    private function throwHttpError($code){
        $httpErrors=array(
            "401"=>"You should not be here",
            "403"=>"Authentication failed",
            "500"=>"Service not available"
        );
        header("HTTP/1.1 ".$code." ".$httpErrors[$code]);
        echo "<h1 style='color:red'>".$code."</h1><h3>".$httpErrors[$code]."</h3>";
        die();
    }
}

?>
