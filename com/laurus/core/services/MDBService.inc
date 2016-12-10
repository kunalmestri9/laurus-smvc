<?php

import("com.laurus.core.connection.CoreConnection");
import("com.laurus.core.services.TQueryList");
import("com.laurus.core.mail.api.IMailService");
import("com.laurus.core.collection.SimpleList");
import("com.laurus.core.collection.map.ExMap");
import("com.laurus.core.json.JsonToObjectConverter");
import("com.laurus.core.coreent.BeanAttr");
import("com.laurus.core.security.encoders.QueryEncoder");



/**
 * Description of MDBService
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
abstract class MDBService {

    private static $instance;
    private $logger;
    private $conn;

    public function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
        $this->conn = new CoreConnection();
        $this->conn->setConnectionType(IConnectionType::SIMPLECONNECTION);
        
    }
    public function getStatementForCustomQuery(){
        return $this->conn->createStatement();
    }
    protected function updateToDb($bean){
        //First Fetch class from the bean;
        $className=get_class($bean);
        //Then call getDocComments on that 
        $refClass = new ReflectionClass($className);
        //get annotation of @tablename 
        $classComments=$refClass->getDocComment();
        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
                list(, $tableName) = $matches;
        }
        //Create a blank query;
        $query="UPDATE " . $tableName ." SET ";
        //Get all properties and create query
        //Create Key Pair for column names and value pair for values if not null
        $map=new Map();
        $primaryProperty=null;
        
        $primaryBeanAttr=null;
        foreach ($refClass->getProperties() as $refProperty) {
            $beanAttr=new BeanAttr();
            //Getting database column name of the property
            if (preg_match('/@columnName\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) { 
                list(, $columnName) = $matches;
                 $this->logger->debug("COL=>".$columnName);
            }else{
                //no need to update
                continue;
            }
            
            //Get the type of the key as it matters now.
            if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                list(, $dbtype) = $matches;
            //Get the value of the property
            $val=$this->getValueOfProperty($refClass,$refProperty,$bean);
            $this->logger->debug("VAL=>".$val);
            //Is it the primary key
            if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Primary Key set");
                $primaryBeanAttr=new BeanAttr();
                $primaryProperty=$refProperty->getName(); //saving the primary property for returning new inserted id
                if(empty($val)|| $val==""){
                   //Cant update row here update required primary value
                   //throw someexception from here
                }else{
                    $primaryBeanAttr
                        ->setKey($columnName)
                        ->setVal($val)
                        ->setType($dbtype);
                    
                }
                continue; 
            }else {
                //Is it the not primary key then is it mandatory
                if (preg_match('/@mandatory\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                    $isMandatory = $true;
                
                if($isMandatory){
                   //Do something for mandatory
                    if(($val==NULL || $val=="") && $val!==0){
                        //From here it should throw exception 
                        $this->logger->debug("val is empty");
                    }else{
                        //$map->put( $columnName,NULL);
                        $map->put($columnName ,$beanAttr->setKey($columnName)->setVal($val)->setType($dbtype));
                    }
                   
                }else{
                    if(($val==NULL || $val=="") && $val!==0){
                        //NO exception just skip the column
                        $this->logger->debug("val is empty _ Non mandatory"); 
                    }else{
                        $map->put($columnName ,$beanAttr->setKey($columnName)->setVal($val)->setType($dbtype));
                        //$map->put( $columnName,$val);
                        //$this->logger->debug($columnName);
                        $this->logger->debug($val);
                    }
                }
            }
            // HERE Query should be 
            // INSERT INTO TABLE NAME (COLUMNANME) VALUES (@ID)
        }

        
        $iterator=$map->getIterator();
        //Here Manupulation of query starts
        //Need few security checks here.
        while($iterator->hasNext()){
            $entry=$iterator->next();
            //Setting up the column
            $key=$entry->getKey();
           
            //Set up the value;
            $value=$entry->getValue();
            if($value->getType()=="int" || empty($value->getType())|| $value->getType()==""){
                $valueString=$value->getVal();
            }else if($value->getType()=="string"){
                $valueString="'" .QueryEncoder::encodeString($value->getVal(), $this->conn) ."'" ;
            }
            
            $queryPart.=$key."=".$valueString;
            if($iterator->hasNext()){
                $queryPart.=",";
            }
        }
        
         //Time to create query
        if($primaryBeanAttr->getType()=="int" || empty($primaryBeanAttr->getType())|| $primaryBeanAttr->getType()==""){
            $primaryKeyValue=$primaryBeanAttr->getVal();
        }else if($primaryBeanAttr->getType()=="string"){
            $primaryKeyValue="'" . QueryEncoder::encodeString($primaryBeanAttr->getVal() , $this->conn) ."'" ;
        }
        //set up the query
        $query=$query.$queryPart." WHERE " . $primaryBeanAttr->getKey(). "=".$primaryKeyValue;
        $this->logger->debug($query);
        
        $id=$this->executeDML($query);
        return $id;
    }
    
    
    protected function addToDb($bean){
        //First Fetch class from the bean;
        $className=get_class($bean);
        //Then call getDocComments on that 
        $refClass = new ReflectionClass($className);
        //get annotation of @tablename 
        $classComments=$refClass->getDocComment();
        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
                list(, $tableName) = $matches;
        }
        //Create a blank query;
        $query="INSERT INTO " . $tableName;
        //Get all properties and create query
        //Create Key Pair for column names and value pair for values if not null
        $map=new Map();
        $primaryProperty=null;
            

        foreach ($refClass->getProperties() as $refProperty) {
            $beanAttr=new BeanAttr();
            //Getting database column name of the property
            if (preg_match('/@columnName\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) { 
                list(, $columnName) = $matches;
                 $this->logger->debug("COL=>".$columnName);
            }else{
                //no need to update
                continue;
            }
            
            //Get the type of the key as it matters now.
            if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                list(, $dbtype) = $matches;
            //Get the value of the property
            $val=$this->getValueOfProperty($refClass,$refProperty,$bean);
            $this->logger->debug("VAL=>".$val);
            //Is it the primary key
            if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Primary Key set");
                $primaryProperty=$refProperty->getName(); //saving the primary property for returning new inserted id
                if(empty($val)|| $val==""){
                    $map->put($columnName ,$beanAttr->setKey($columnName)->setVal("@id")); //if user has autoincrement option at the db
                }else{
                    $map->put($columnName ,$beanAttr->setKey($columnName)->setVal($val)->setType($dbtype));
                }
                continue; 
            }else {
                //Is it the not primary key then is it mandatory
                if (preg_match('/@mandatory\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                    $isMandatory = $true;
               
                if($isMandatory){
                   //Do something for mandatory
                    if(($val==NULL || $val=="") && $val!==0){
                        //From here it should throw exception 
                        $this->logger->debug("val is empty");
                    }else{
                        //$map->put( $columnName,NULL);
                        $map->put($columnName ,$beanAttr->setKey($columnName)->setVal($val)->setType($dbtype));
                    }
                   
                }else{
                    
                    if(($val==NULL || $val=="") && $val!==0){
                        //NO exception just skip the column
                        $this->logger->debug("val is empty _ Non mandatory"); 
                    }else if(preg_match('/@skipinsert\s+([^\s]+)/', $refProperty->getDocComment(), $matches)){
                    //From here it should throw exception 
                     $this->logger->debug("skip from insert query");
                    }else{
                        $map->put($columnName ,$beanAttr->setKey($columnName)->setVal($val)->setType($dbtype));
                        //$map->put( $columnName,$val);
                        //$this->logger->debug($columnName);
                        $this->logger->debug($val);
                    }
                }
            }
            // HERE Query should be 
            // INSERT INTO TABLE NAME (COLUMNANME) VALUES (@ID)
        }

        $iterator=$map->getIterator();
        //Here Manupulation of query starts
        //Need few security checks here.
        $keyString=" (";
        $valueString=" (";
        while($iterator->hasNext()){
            $entry=$iterator->next();
            //Setting up the column
            $key=$entry->getKey();
            $keyString.=$key;
            //Set up the value;
            $value=$entry->getValue();
            if($value->getType()=="int" || empty($value->getType())|| $value->getType()==""){
                $valueString.=$value->getVal();
            }else if($value->getType()=="string"){
                $valueString.="'" . QueryEncoder::encodeString($value->getVal(), $this->conn) ."'" ;
            }
            
            if($iterator->hasNext()){
                $keyString.=",";
                $valueString.=",";
            }
        }
        $keyString.=")";
        $valueString.=")";
        
        $query=$query.$keyString." VALUES ".$valueString;
        $this->logger->debug($query);
        
        $id=$this->executeDML($query);
        if($refProperty!=null){
            $this->setValueOfProperty($refClass,$refClass->getProperty($primaryProperty),$bean,$id);
        }
        return $bean;
    }    
    
    
    protected function deleteFromDb($bean){
        //First Fetch class from the bean;
        $className=get_class($bean);
        //Then call getDocComments on that 
        $refClass = new ReflectionClass($className);
        //get annotation of @tablename 
        $classComments=$refClass->getDocComment();
        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
                list(, $tableName) = $matches;
        }
        //Create a blank query;
        $query="DELETE FROM " . $tableName ." WHERE ";
        
        
        $beanAttr=null; 
        foreach ($refClass->getProperties() as $refProperty) {
            //declaring it null easy for checking ahead
            //Getting database column name of the property
            if (preg_match('/@columnName\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) { 
                list(, $columnName) = $matches;
                 $this->logger->debug("COL=>".$columnName);
                //Is it the primary key
                if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                    $this->logger->debug("Primary Key found");
                    $beanAttr=new BeanAttr();
                    //Get the type of the key as it matters now.
                    if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                        list(, $dbtype) = $matches;
                    //Get the value of the property
                    $val=$this->getValueOfProperty($refClass,$refProperty,$bean);
                    $this->logger->debug("VAL=>".$val);
                    if(empty($val)|| $val==""){
                        //Primary key value empty in the delete not possible
                        //throw Some Exception here
                       $this->logger->debug("Primary key value empty in the delete!!! not possible");

                    }else{
                        $beanAttr
                            ->setKey($columnName)
                            ->setVal($val)
                            ->setType($dbtype);
                        //ohk found which i was looking for now break
                        break;
                    }
                    continue; //I dont think this is need but still keeping it
                }else{
                    //Property is not primary 
                    continue; 
                }     
            }else{
                //some other property not related to database
                continue;
            }
        }
        if($beanAttr==null){
            //no primary key found in bean
            //that it odd should throw error from here
            $this->logger->debug("that it odd should throw error from here");
        }else{
            //Time to create query
            if($beanAttr->getType()=="int" || empty($beanAttr->getType())|| $beanAttr->getType()==""){
                $primaryKeyValue=$beanAttr->getVal();
            }else if($beanAttr->getType()=="string"){
                $primaryKeyValue="'" . QueryEncoder::encodeString($beanAttr->getVal(), $this->conn)  ."'" ;
            }
            //set up the query
            $query.=$beanAttr->getKey()."=".$primaryKeyValue;
        }
        $this->logger->debug($query);
        
        $this->executeDML($query);
    }
    
    
    protected function addListToDb(\SimpleList $list,$isMultiInsert=false){
        if($isMultiInsert){
            $this->addListToDbMultiInsert($list);
        }else{
            for ($index = 0; $index < $list->size(); $index++) {
                $bean=$list->get($index);
                $this->addToDb($bean);
                $this->logger->debug($bean);
            }
        }
    }
    
    protected function findById($bean,$isCompositeResponseNeeded=false){
        //First Fetch class from the bean;
        $className=get_class($bean);
        //Then call getDocComments on that 
        $refClass = new ReflectionClass($className);
        //get annotation of @tablename 
        $classComments=$refClass->getDocComment();
        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
                list(, $tableName) = $matches;
        }
        // Main class table name
        //$tableName;
        
        //Get All the ColumnList for the First Table 
        
        $columnMap=new ExMap();
        $compositeBeans=new ExMap();
        $primaryProperty=null;
        $searchablePropertyList=new SimpleList();
          $injectablesList=new SimpleList();

        //iterate through the entity and identify if there is any composite response required present
        foreach ($refClass->getProperties() as $refProperty) {
            $isCompositeEntity=false;
            $isPrimaryPrperty=false;
            $isSearchableAttribute=false;
            $beanAttr=new BeanAttr();
            $this->logger->debug("beanAttr");
            $this->logger->debug($beanAttr);
            //Getting database column name of the property
            if (preg_match('/@compositeEntity\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                list(, $compositeDependancy) = $matches;

                $this->logger->debug("compositeEntity Found");
                $isCompositeEntity=true;
            }
              if (preg_match('/@injectable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("injectable Attribute Found");
                 if (preg_match('/@returnType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_returnType) = $matches;
                $beanAttr->setReturnType($_returnType);
                 $beanAttr->setPropertyRefference($refProperty);
                $this->logger->debug($beanAttr);
                $injectablesList->add($beanAttr);
                $isInjectable=true;
                
            } 

            if (preg_match('/@columnName\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) { 
                list(, $columnName) = $matches;
                 $this->logger->debug("COL=>".$columnName);
            }else{
                if($isCompositeEntity){
                        //Need to find better place to write it. its REDUNDANT
                        if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $dbtype) = $matches;
                        if (preg_match('/@compositeType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_compositeType) = $matches;
                    $beanAttr->setKey($compositeDependancy)->setPropertyRefference($refProperty)->setType($dbtype)->setCompositeType($_compositeType);
                    $compositeBeans->put($compositeDependancy ,$beanAttr);

                }
                //no need to update
                continue;
            }
            
            //Get the type of the key as it matters now.
            if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                list(, $dbtype) = $matches;
            //Get the value of the property
            $val=$this->getValueOfProperty($refClass,$refProperty,$bean);
            $this->logger->debug("VAL=>".$val);
            //Is it the primary key
            if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Primary Key set");
                $isPrimaryPrperty=true;//saving the primary property for returning new inserted id
            }else if (preg_match('/@searchable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Searchable Attribute Found");
                $isSearchableAttribute=true;
            }
            
            
            $beanAttr->setKey($columnName)->setPropertyRefference($refProperty)->setType($dbtype);
            if($isPrimaryPrperty){
                $primaryProperty=$beanAttr;
            }else if($isSearchableAttribute){
                $searchablePropertyList->add($beanAttr);
            }
            
            $columnMap->put($columnName ,$beanAttr);
            if($isCompositeEntity){
                if (preg_match('/@compositeType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_compositeType) = $matches;
                $beanAttr->setCompositeType($_compositeType);
                $compositeBeans->put($columnName ,$beanAttr);
            }
            
        }
            // HERE Query should be 
            // SELECT * FROM TABLE NAME WHERE ID = ?

        if(method_exists($bean, "getSelectionColumn") && $bean->getSelectionColumn()!=""){
            $query="SELECT ".$bean->getSelectionColumn()." FROM ". $tableName . " WHERE " ;
        }else{
            $query="SELECT * FROM ". $tableName . " WHERE " ;
        }
        
        $prmVal=$this->getValueOfProperty($refClass, $primaryProperty->getPropertyRefference(), $bean);
        if($prmVal==""){
            for ($index = 0; $index < $searchablePropertyList->size(); $index++) {
                $prop=$searchablePropertyList->get($index);
                $val=$this->getValueOfProperty($refClass, $prop->getPropertyRefference(), $bean);
                if($val!=""){
                    $query.= $prop->getKey(). "=" ;
                    if($prop->getType()=="string"){
                        $query.="'".QueryEncoder::encodeString($val, $this->conn) ."'";
                    }else{
                        $query.=$val;
                    }
                    if($searchablePropertyList->size()!=($index+1)){
                        $query.=" AND ";
                    }
                }
            }
            $query=rtrim($query, " AND ");
        }else{
            $query.= $primaryProperty->getKey(). "=" ;
            if($primaryProperty->getType()=="string"){
                $this->logger->debug("Select chya aat");
                $this->logger->debug(QueryEncoder::encodeString($prmVal, $this->conn));
                $query.="'".QueryEncoder::encodeString($prmVal, $this->conn)."'";
            }else{
                $query.=$prmVal;
            }
        }
        if(method_exists($bean, "getAppendQuery") && $bean->getAppendQuery()!=""){
             $query.= " ".$bean->getAppendQuery();
        }
        
        
        
        
        $resultSet=$this->execute($query);
        $keys=$columnMap->getKeys();
        if(!$resultSet->hasRecords()){
            throw new SqlException("No records found",SQLExceptionType::ZERO_ROWS);
        }
        
        $valueMap=new Map();
        
        if($resultSet->next()){
            for ($i=0; $i < count($keys) ; $i++) { 
                $val=$resultSet->get($keys[$i]);
                /* @var $columnBeanAttr BeanAttr */
                $columnBeanAttr=$columnMap->getValue($keys[$i]);
                $propertyRef=$columnBeanAttr->getPropertyRefference();
                $this->setValueOfProperty($refClass, $propertyRef, $bean, $val);
                $valueMap->put($keys[$i], $val);
            }
        }
        if($isCompositeResponseNeeded){
            $keys1=$compositeBeans->getKeys();
            for ($index1 = 0; $index1 < count($keys1); $index1++) {
                 /*@var $compositeAttribute BeanAttr*/
                 $compositeAttribute=$compositeBeans->get($keys1[$index1]);
                 $type=$compositeAttribute->getCompositeType(); //Get Type from it
                 $propRefClass=new ReflectionClass($type); //Get Blueprint of the class. Its expected that ClassLoader has already found the class which is needed
                 $instance=$propRefClass->newInstance();// Bean must have default constructor
                 $val=$valueMap->getValue($keys1[$index1]); //Getting value fetch from the DB
                 if($val!=null){
                    $this->setPrimaryProperty($propRefClass, $instance, $val);
                    $this->logger->debug($instance);
                    $this->findById($instance);
                    $this->setValueOfProperty($refClass, $compositeAttribute->getPropertyRefference(), $bean, $instance);//Setting up the value in the bean
                 }
                 
            } 
        }
        
        
         $this->logger->debug($bean); 
           $this->logger->debug("Injectable if madhe");
                for($i=0;$i<$injectablesList->size();$i++){
                      /*@var $val BeanAttr*/
                    $val=$injectablesList->get($i);
                    $returnType=$val->getReturnType();
                    $this->logger->debug("Injectable if madhe");
                    $this->logger->debug($val);
                    if($returnType==1){
                        $reflectionProperty=$val->getPropertyRefference();
                        $injectableMethod="getInjectable".ucfirst($reflectionProperty->getName());
                         
                        $instance=$bean->$injectableMethod();
                        $this->logger->debug($instance);
                        try{
                            $setinjectableMethod="setInjectable".ucfirst($reflectionProperty->getName());
                            $this->findById($instance,true);
                            $bean->$setinjectableMethod($instance);

                        }  catch (\SQLException $ex){
                             if ($ex->getCode() === SQLExceptionType::ZERO_ROWS) {
                                $this->logger->debug("skipped zero rows affected");
                                //throw new ValidationException("App Id Not Found", IAppvityExceptionType::APP_ID_NOT_FOUND); 
                        }
                        }
                        
                    }else{
                         $reflectionProperty=$val->getPropertyRefference();
                        $injectableMethod="getInjectable".ucfirst($reflectionProperty->getName());
                         
                        $instance=$bean->$injectableMethod();
                        $this->logger->debug($instance);
                        try{
                            $setinjectableMethod="setInjectableList".ucfirst($reflectionProperty->getName());
                            $list=$this->findCollectionById($instance,true);
                            $injectObjectList=new SimpleList();
                            $this->logger->debug($list);
                            for($j=0;$j<$list->size();$j++){
                               $injectObjectList->add($list->get($j));
                            }
                            $bean->$setinjectableMethod($injectObjectList);

                        }  catch (\SQLException $ex){
                             if ($ex->getCode() === SQLExceptionType::ZERO_ROWS) {
                                $this->logger->debug("skipped zero rows affected");
                                //throw new ValidationException("App Id Not Found", IAppvityExceptionType::APP_ID_NOT_FOUND); 
                        }
                        }
                    }
                }
        
        
        
    }
    
    
    
    protected function findCollectionById($bean,$isCompositeResponseNeeded=false){
         //First Fetch class from the bean;
        $className=get_class($bean);
        //Then call getDocComments on that 
        $refClass = new ReflectionClass($className);
        //get annotation of @tablename 
        $classComments=$refClass->getDocComment();
        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
                list(, $tableName) = $matches;
        }
        // Main class table name
        //$tableName;
        
        //Get All the ColumnList for the First Table 
        
        $columnMap=new ExMap();
        $compositeBeans=new ExMap();
        $primaryProperty=null;
        $searchablePropertyList=new SimpleList();
        $injectablesList=new SimpleList();

        //iterate through the entity and identify if there is any composite response required present
        foreach ($refClass->getProperties() as $refProperty) {
            $isCompositeEntity=false;
            $isPrimaryPrperty=false;
            $isSearchableAttribute=false;
            $isInjectable=false;
            $beanAttr=new BeanAttr();
           
            //Getting database column name of the property
            if (preg_match('/@compositeEntity\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                list(, $compositeDependancy) = $matches;

                $this->logger->debug("compositeEntity Found");
                $isCompositeEntity=true;
            }

             if (preg_match('/@injectable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("injectable Attribute Found");
                 if (preg_match('/@returnType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_returnType) = $matches;
                $beanAttr->setReturnType($_returnType);
                 $beanAttr->setPropertyRefference($refProperty);
                $this->logger->debug($beanAttr);
                $injectablesList->add($beanAttr);
                $isInjectable=true;
                
            } 
            if (preg_match('/@columnName\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) { 
                list(, $columnName) = $matches;
                 $this->logger->debug("COL=>".$columnName);
            }else{
                if($isCompositeEntity){
                        //Need to find better place to write it. its REDUNDANT
                        if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $dbtype) = $matches;
                        if (preg_match('/@compositeType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_compositeType) = $matches;
                    $beanAttr->setKey($compositeDependancy)->setPropertyRefference($refProperty)->setType($dbtype)->setCompositeType($_compositeType);
                    $compositeBeans->put($compositeDependancy ,$beanAttr);

                }
                //no need to update
                continue;
            }
            
            //Get the type of the key as it matters now.
            if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                list(, $dbtype) = $matches;
            //Get the value of the property
            $val=$this->getValueOfProperty($refClass,$refProperty,$bean);
            $this->logger->debug("VAL=>".$val);
            //Is it the primary key
            if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Primary Key set");
                $isPrimaryPrperty=true;//saving the primary property for returning new inserted id
            }else if (preg_match('/@searchable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Searchable Attribute Found");
                $isSearchableAttribute=true;
            }
            
            
            $beanAttr->setKey($columnName)->setPropertyRefference($refProperty)->setType($dbtype);
            if($isPrimaryPrperty){
                $primaryProperty=$beanAttr;
            }else if($isSearchableAttribute && TUtilString::isValidString($val)){
                $searchablePropertyList->add($beanAttr);
            }
            
            $columnMap->put($columnName ,$beanAttr);
            if($isCompositeEntity){
                if (preg_match('/@compositeType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_compositeType) = $matches;
                $beanAttr->setCompositeType($_compositeType);
                $compositeBeans->put($columnName ,$beanAttr);
            }
            
//            if (preg_match('/@injectable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)){
//                $this->logger->debug("injectable");
//                 if (preg_match('/@returnType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
//                         list(, $_returnType) = $matches;
//                $beanAttr->setReturnType($_returnType);
//                $this->logger->debug($beanAttr);
//                $injectablesList->add($beanAttr);
//                $isInjectable=true;
//            }    
            
        }
            // HERE Query should be 
            // SELECT * FROM TABLE NAME WHERE ID = ?

        
        $query="SELECT * FROM ". $tableName ;
        $prmVal=$this->getValueOfProperty($refClass, $primaryProperty->getPropertyRefference(), $bean);
        $query.=($searchablePropertyList->size()>0) && ($prmVal=="") ?" WHERE ":"";
        if($prmVal==""){
//            //shreepad
//            if($searchablePropertyList->size()>0){
//                 $query=$query. " WHERE " ;
//            }
//            //shreepad
            for ($index = 0; $index < $searchablePropertyList->size(); $index++) {
                $prop=$searchablePropertyList->get($index);
                $val=$this->getValueOfProperty($refClass, $prop->getPropertyRefference(), $bean);
                if($val!=""){
                    $query.= $prop->getKey(). "=" ;
                    if($prop->getType()=="string"){
                        $query.="'".QueryEncoder::encodeString($val, $this->conn) ."'";
                    }else{
                        $query.=$val;
                    }
                    if($searchablePropertyList->size()!=($index+1)){
                        $query.=" AND ";
                    }
                }
            }
            $query=rtrim($query, " AND ");
        }else{
            $query.= $primaryProperty->getKey(). "=" ;
            if($primaryProperty->getType()=="string"){
                $query.="'".QueryEncoder::encodeString($prmVal, $this->conn) ."'";
            }else{
                $query.=$prmVal;
            }
        }
        
        
        
        
        $resultSet=$this->execute($query);
        $keys=$columnMap->getKeys();
        if(!$resultSet->hasRecords()){
            throw new SqlException("No records found",SQLExceptionType::ZERO_ROWS);
        }
        
        
        $returnList=new SimpleList();
        while($resultSet->next()){
            $valueMap=new Map();
            $beanInstance=$refClass->newInstance();
            for ($i=0; $i < count($keys) ; $i++) { 
                $val=$resultSet->get($keys[$i]);
                /* @var $columnBeanAttr BeanAttr */
                $columnBeanAttr=$columnMap->getValue($keys[$i]);
                $propertyRef=$columnBeanAttr->getPropertyRefference();
                $this->setValueOfProperty($refClass, $propertyRef, $beanInstance, $val);
                $valueMap->put($keys[$i], $val);
                
            }
            if($isCompositeResponseNeeded){
                $keys1=$compositeBeans->getKeys();
                for ($index1 = 0; $index1 < count($keys1); $index1++) {
                     $val=$valueMap->getValue($keys1[$index1]); //Getting value fetch from the DB
                      $this->logger->debug("Getting value fetch from the DB");
                     $this->logger->debug($val);
                     if($val!=""){
                         $this->logger->debug($keys1[$index1]); 
                        /*@var $compositeAttribute BeanAttr*/
                        $compositeAttribute=$compositeBeans->get($keys1[$index1]);
                        $type=$compositeAttribute->getCompositeType(); //Get Type from it
                        $propRefClass=new ReflectionClass($type); //Get Blueprint of the class. Its expected that ClassLoader has already found the class which is needed
                        $instance=$propRefClass->newInstance();// Bean must have default constructor
                        $this->setPrimaryProperty($propRefClass, $instance, $val);
                        $this->logger->debug($instance);
                        $this->findById($instance,$isCompositeResponseNeeded);
                        $this->setValueOfProperty($refClass, $compositeAttribute->getPropertyRefference(), $beanInstance, $instance);//Setting up the value in the bean

                     }
                } 
            }
           $this->logger->debug($beanInstance); 
           $this->logger->debug("Injectable if madhe");
                for($i=0;$i<$injectablesList->size();$i++){
                      /*@var $val BeanAttr*/
                    $val=$injectablesList->get($i);
                    $returnType=$val->getReturnType();
                    $this->logger->debug("Injectable if madhe");
                    $this->logger->debug($val);
                    if($returnType==1){
                        $reflectionProperty=$val->getPropertyRefference();
                        $injectableMethod="getInjectable".ucfirst($reflectionProperty->getName());
                         
                        $instance=$bean->$injectableMethod();
                        $this->logger->debug($instance);
                        try{
                            $setinjectableMethod="setInjectable".ucfirst($reflectionProperty->getName());
                            $this->findById($instance,true);
                            $bean->$setinjectableMethod($instance);

                        }  catch (\SQLException $ex){
                             if ($ex->getCode() === SQLExceptionType::ZERO_ROWS) {
                                $this->logger->debug("skipped zero rows affected");
                                //throw new ValidationException("App Id Not Found", IAppvityExceptionType::APP_ID_NOT_FOUND); 
                        }
                        }
                        
                    }else{
                         $reflectionProperty=$val->getPropertyRefference();
                        $injectableMethod="getInjectable".ucfirst($reflectionProperty->getName());
                         
                        $instance=$bean->$injectableMethod();
                        $this->logger->debug($instance);
                        try{
                            $setinjectableMethod="setInjectableList".ucfirst($reflectionProperty->getName());
                            $list=$this->findCollectionById($instance,true);
                            $injectObjectList=new SimpleList();
                            $this->logger->debug($list);
                            for($j=0;$j<$list->size();$j++){
                               $injectObjectList->add($list->get($j));
                            }
                            $bean->$setinjectableMethod($injectObjectList);

                        }  catch (\SQLException $ex){
                             if ($ex->getCode() === SQLExceptionType::ZERO_ROWS) {
                                $this->logger->debug("skipped zero rows affected");
                                //throw new ValidationException("App Id Not Found", IAppvityExceptionType::APP_ID_NOT_FOUND); 
                        }
                        }
                    }
                }
           
            $returnList->add($beanInstance);
        }
        return $returnList;
        
    }
    
//    private function compairJoinsForSequencing(JoinInfo $a,\JoinInfo  $b){
//        if (((int)$a->getSequence())== ((int)$b->getSequence())) 
//            return 0;
//        return (((int)$a->getSequence()) < ((int)$b->getSequence())) ? -1 : 1;
//    }
//    
//    public function getAllDataWithJoin($bean,$joinName){
//       $list=$this->getAllJoinData($bean, $joinName);
//       $list->sort($this->compairJoinsForSequencing);
//       $this->logger->debug($list);
//       if($list->size()>1 ){
//           $query="SELECT * FROM ";
//            for ($index = 0; $index < $list->size(); $index++) {
//                /*@var $obj JoinInfo*/
//                $obj=$list->get($index);
//                $query.=$obj->getTableName(). " ";
//                if($obj->getJoinType()!=""){
//                    $query.=$obj->getJoinType()." JOIN ";
//                }
//                
//
//            }
//       }
//       
//    }
//    
//    public function getAllJoinData($bean,$joinName){
//        $this->logger->debug(__METHOD__);
//        //First Fetch class from the bean;
//        $className=get_class($bean);
//        //Then call getDocComments on that 
//        $refClass = new ReflectionClass($className);
//        $classComments=$refClass->getDocComment();
//        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
//                list(, $tableName) = $matches;
//        }
//        
//        $joinList=new SimpleList();
//        
//        foreach ($refClass->getProperties() as $refProperty) {
//            if (preg_match('/@join\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
//                list(, $joinInfo) = $matches;
//                //@joinName SPECS_WITH_FILES|{"onBeanType":File,"joinType":"INNER","sequence":"1"}  
//                //private $currentBeanType;
//                /*
//                private $onBeanType;
//                private $joinType;
//                private $joinName;
//                private $sequence;
//                 */
//                $this->logger->debug($joinInfo);
//                $joinInfoArray=explode("|",$joinInfo);
//                $this->logger->debug($joinInfoArray);
//                if($joinInfoArray[0]==$joinName){ //check if the Join is the same one
//                    if(is_array(json_decode($joinInfoArray[1],true))){
//                        //Ok format seems to be right
//                        //Convert the string into json
//                        $converter = new JsonToObjectConverter();
//                        /*@var $joinInfo JoinInfo */
//                        $joinInfo=$converter->convertToObject($joinInfoArray[1], "com.laurus.core.coreent.JoinInfo");
//                        $joinInfo->setJoinName($joinInfoArray[0])
//                                ->setOnRefferenceProperty($refProperty)
//                                ->setTableName($tableName);
//                        $this->logger->debug("===============JOIN INFO===============");
//                        $this->logger->debug($joinInfo);
//                        
//                        $joinList->add($joinInfo);
//                        
//                        if($joinInfo->getOnBeanType()!=""){
//                            $newClass=(new ReflectionClass($joinInfo->getOnBeanType()));
//                            $inst=$newClass->newInstance();
//                            $list=$this->getAllJoinData($inst, $joinName);
//                            $joinList->addArray($list->toArray());
//                        }
//                    }else{
//                        $this->logger->debug("Invalid JSON format");
//                    }
//                    //Now just create Join Bean with Sequence Id;
//                    //$map->put($key, $value);
//                }
//            }
//        }
//        return $joinList;
//        
//    }
//    
    
    protected function getByCustomQuery($bean,$query,$isCompositeResponseNeeded=false){
        if(!TUtilString::isValidString($query)){
            //should throw error here 
            
        }
        
        //First Fetch class from the bean;
        $className=get_class($bean);
        //Then call getDocComments on that 
        $refClass = new ReflectionClass($className);
        //get annotation of @tablename 
        $classComments=$refClass->getDocComment();
        if (preg_match('/@tableName\s+([^\s]+)/', $classComments, $matches)) {
                list(, $tableName) = $matches;
        }
        // Main class table name
        //$tableName;
        
        //Get All the ColumnList for the First Table 
        
        $columnMap=new ExMap();
        $compositeBeans=new ExMap();
        $primaryProperty=null;
        $searchablePropertyList=new SimpleList();
          $injectablesList=new SimpleList();

        //iterate through the entity and identify if there is any composite response required present
        foreach ($refClass->getProperties() as $refProperty) {
            $isCompositeEntity=false;
            $isPrimaryPrperty=false;
            $isSearchableAttribute=false;
            $beanAttr=new BeanAttr();
            $this->logger->debug("beanAttr");
            $this->logger->debug($beanAttr);
            //Getting database column name of the property
            if (preg_match('/@compositeEntity\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                list(, $compositeDependancy) = $matches;

                $this->logger->debug("compositeEntity Found");
                $isCompositeEntity=true;
            }
            if (preg_match('/@injectable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("injectable Attribute Found");
                 if (preg_match('/@returnType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_returnType) = $matches;
                $beanAttr->setReturnType($_returnType);
                 $beanAttr->setPropertyRefference($refProperty);
                $this->logger->debug($beanAttr);
                $injectablesList->add($beanAttr);
                $isInjectable=true;
                
            } 

            if (preg_match('/@columnName\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) { 
                list(, $columnName) = $matches;
                 $this->logger->debug("COL=>".$columnName);
            }else{
                if($isCompositeEntity){
                        //Need to find better place to write it. its REDUNDANT
                        if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $dbtype) = $matches;
                        if (preg_match('/@compositeType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_compositeType) = $matches;
                    $beanAttr->setKey($compositeDependancy)->setPropertyRefference($refProperty)->setType($dbtype)->setCompositeType($_compositeType);
                    $compositeBeans->put($compositeDependancy ,$beanAttr);

                }
                //no need to update
                continue;
            }
            
            //Get the type of the key as it matters now.
            if (preg_match('/@type\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                list(, $dbtype) = $matches;
            //Get the value of the property
            $val=$this->getValueOfProperty($refClass,$refProperty,$bean);
            $this->logger->debug("VAL=>".$val);
            //Is it the primary key
            if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Primary Key set");
                $isPrimaryPrperty=true;//saving the primary property for returning new inserted id
            }else if (preg_match('/@searchable\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->logger->debug("Searchable Attribute Found");
                $isSearchableAttribute=true;
            }
            
            
            $beanAttr->setKey($columnName)->setPropertyRefference($refProperty)->setType($dbtype);
            if($isPrimaryPrperty){
                $primaryProperty=$beanAttr;
            }else if($isSearchableAttribute){
                $searchablePropertyList->add($beanAttr);
            }
            
            $columnMap->put($columnName ,$beanAttr);
            if($isCompositeEntity){
                if (preg_match('/@compositeType\s+([^\s]+)/', $refProperty->getDocComment(), $matches))
                         list(, $_compositeType) = $matches;
                $beanAttr->setCompositeType($_compositeType);
                $compositeBeans->put($columnName ,$beanAttr);
            }
            
        }
        // HERE Query should be 
        // SELECT * FROM TABLE NAME WHERE ID = ?

        
        $resultSet=$this->execute($query);
        $keys=$columnMap->getKeys();
        if(!$resultSet->hasRecords()){
            throw new SqlException("No records found",SQLExceptionType::ZERO_ROWS);
        }
        
        $valueMap=new Map();
        
        $returnList=new SimpleList();
        while($resultSet->next()){
            $valueMap=new Map();
            $beanInstance=$refClass->newInstance();
            for ($i=0; $i < count($keys) ; $i++) { 
                $val=$resultSet->get($keys[$i]);
                /* @var $columnBeanAttr BeanAttr */
                $columnBeanAttr=$columnMap->getValue($keys[$i]);
                $propertyRef=$columnBeanAttr->getPropertyRefference();
                $this->setValueOfProperty($refClass, $propertyRef, $beanInstance, $val);
                $valueMap->put($keys[$i], $val);
                
            }
            if($isCompositeResponseNeeded){
                $keys1=$compositeBeans->getKeys();
                for ($index1 = 0; $index1 < count($keys1); $index1++) {
                     $val=$valueMap->getValue($keys1[$index1]); //Getting value fetch from the DB
                      $this->logger->debug("Getting value fetch from the DB");
                     $this->logger->debug($val);
                     if($val!=""){
                         $this->logger->debug($keys1[$index1]); 
                        /*@var $compositeAttribute BeanAttr*/
                        $compositeAttribute=$compositeBeans->get($keys1[$index1]);
                        $type=$compositeAttribute->getCompositeType(); //Get Type from it
                        $propRefClass=new ReflectionClass($type); //Get Blueprint of the class. Its expected that ClassLoader has already found the class which is needed
                        $instance=$propRefClass->newInstance();// Bean must have default constructor
                        $this->setPrimaryProperty($propRefClass, $instance, $val);
                        $this->logger->debug($instance);
                        $this->findById($instance,$isCompositeResponseNeeded);
                        $this->setValueOfProperty($refClass, $compositeAttribute->getPropertyRefference(), $beanInstance, $instance);//Setting up the value in the bean

                     }
                } 
            }
           $this->logger->debug($beanInstance); 
           $this->logger->debug("Injectable if madhe");
                for($i=0;$i<$injectablesList->size();$i++){
                      /*@var $val BeanAttr*/
                    $val=$injectablesList->get($i);
                    $returnType=$val->getReturnType();
                    $this->logger->debug("Injectable if madhe");
                    $this->logger->debug($val);
                    if($returnType==1){
                        $reflectionProperty=$val->getPropertyRefference();
                        $injectableMethod="getInjectable".ucfirst($reflectionProperty->getName());
                         
                        $instance=$beanInstance->$injectableMethod();
                        $this->logger->debug($instance);
                        try{
                            $setinjectableMethod="setInjectable".ucfirst($reflectionProperty->getName());
                            $this->findById($instance,true);
                            $beanInstance->$setinjectableMethod($instance);

                        }  catch (\SQLException $ex){
                             if ($ex->getCode() === SQLExceptionType::ZERO_ROWS) {
                                $this->logger->debug("skipped zero rows affected");
                                //throw new ValidationException("App Id Not Found", IAppvityExceptionType::APP_ID_NOT_FOUND); 
                        }
                        }
                        
                    }else{
                         $reflectionProperty=$val->getPropertyRefference();
                        $injectableMethod="getInjectable".ucfirst($reflectionProperty->getName());
                         
                        $instance=$beanInstance->$injectableMethod();
                        $this->logger->debug($instance);
                        try{
                            $setinjectableMethod="setInjectableList".ucfirst($reflectionProperty->getName());
                            $list=$this->findCollectionById($instance,true);
                            $injectObjectList=new SimpleList();
                            $this->logger->debug($list);
                            for($j=0;$j<$list->size();$j++){
                               $injectObjectList->add($list->get($j));
                            }
                            $beanInstance->$setinjectableMethod($injectObjectList);

                        }  catch (\SQLException $ex){
                             if ($ex->getCode() === SQLExceptionType::ZERO_ROWS) {
                                $this->logger->debug("skipped zero rows affected");
                                //throw new ValidationException("App Id Not Found", IAppvityExceptionType::APP_ID_NOT_FOUND); 
                        }
                        }
                    }
                }
            $returnList->add($beanInstance);
        }
        return $returnList;
        
        
        
    }
    private function setPrimaryProperty($refClass,$bean,$value){
        $this->logger->debug(__METHOD__);
        foreach ($refClass->getProperties() as $refProperty) {
            if (preg_match('/@primary\s+([^\s]+)/', $refProperty->getDocComment(), $matches)) {
                $this->setValueOfProperty($refClass, $refProperty, $bean, $value);
                $this->logger->debug($value);
                $this->logger->debug($bean);
                return;
            }
        }
    }    
        
    
    
    private function addListToDbMultiInsert(\SimpleList $list){
        //Yet to code
        
    }
    
    
    private function execute($query){
        $stmt = $this->conn->createStatement();
        $stmt->setQuery($query);
        $result= $this->conn->execute($stmt);
        return $result;
    }
    
    private function executeDML($query){
        $stmt = $this->conn->createStatement();
        $stmt->setQuery($query);
        $id = $this->conn->executeUpdate($stmt, true);
        return $id;
    }
    
    
    public function executeStatementDML($stmt){
        $id = $this->conn->executeUpdate($stmt, true);
        return $id;
    }
    private function getValueOfProperty($refClass,$refProperty,$bean){
        if($refProperty->isPublic()){
            $val=$refProperty->getValue($bean);
        }else{
            $methodName="get" . ucfirst($refProperty->getName());
            $this->logger->debug($methodName);
            if($refClass->hasMethod($methodName)){
                $refMethod=$refClass->getMethod($methodName);
                $val=$refMethod->invoke($bean);
                $this->logger->debug($val);
            }else{
                //throw exception that method not available for private/protected property
            }
        }
        return $val;
    }
    /**
     * 
     * @param type $refClass
     * @param type $refProperty
     * @param type $bean
     * @param type $value
     * @TODO Need to throw exception properly
     */
    private function setValueOfProperty($refClass,$refProperty,$bean,$value){
        if($refProperty->isPublic()){
            $refProperty->setValue($bean,$value);
        }else{
            $methodName="set" . ucfirst($refProperty->getName());
            $this->logger->debug($methodName);
            if($refClass->hasMethod($methodName)){
                $refMethod=$refClass->getMethod($methodName);
                $refMethod->invoke($bean,$value);
            }else{
                //throw exception that method not available for private/protected property
            }
        }
    }

    
    
    
    

      
}

?>