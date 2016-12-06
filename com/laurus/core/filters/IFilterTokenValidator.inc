<?php
/**
 * Description of IFilterTokenValidator
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
interface IFilterTokenValidator {
    /**
     * Description of IFilterTokenValidator
     * 
     * @param String $userName Username of the user t
     * @param String $token token of the user.
     * 
     * @return boolean this method should return true of false based on the users validation
     * 
     * 
     * @throws FilterException Invalid Credentials
     * 
     */
    public function validateUserToken($userName,$token);
}

?>
