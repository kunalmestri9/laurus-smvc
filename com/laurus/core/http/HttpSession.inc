<?php
/**
 * Description of HttpSession
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class HttpSession {
    
    public function invalidate(){
        session_destroy();
    }
    
    public function getSessionId(){
        return session_id();
    }
}

?>
