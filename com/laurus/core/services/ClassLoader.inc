<?php
/**
 * Description of ClassLoader
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class ClassLoader {
    private $class;
    public function __construct($class) {
        $this->class=$class;
    }
    public static function loadName($classpath){
        import($classpath);
        return new ClassLoader(substr(strrchr($classpath,"."),1));
    }
    public function getInstance(){
        return new $this->class;
    }
}

?>
