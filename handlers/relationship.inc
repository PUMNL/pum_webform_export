<?php

class PumWebformExportRelationshipHandler {
  
  public function __construct() {
    
  }
  
  public static function export(&$node, $original_node) {
    $e = new PumWebformExportRelationshipHandler();
    $node->webform_civicrm = $e->checkCiviElements($node->webform_civicrm);
    $node->webform['components'] = $e->checkComponents($node->webform['components']);
  }
  
  public static function import(&$node, $original_node) {
    $e = new PumWebformExportRelationshipHandler();
    $node->webform_civicrm = $e->importCheckCiviElements($node->webform_civicrm);
    $node->webform['components'] = $e->importCheckComponents($node->webform['components']);
  }
  
  public function checkComponents(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      if (isset($element['extra']['default_relationship']) && is_array($element['extra']['default_relationship'])) {
        $element['extra']['default_relationship'] = $this->exportTranslateRelationshipType($element['extra']['default_relationship']);
      }
      
      $return[$key] = $element;
    }
    return $return;
  }
  
  public function importCheckComponents(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      if (isset($element['extra']['default_relationship']) && is_array($element['extra']['default_relationship'])) {
        $element['extra']['default_relationship'] = $this->importTranslateRelationshipType($element['extra']['default_relationship']);
      }
      
      $return[$key] = $element;
    }
    return $return;
  }
  
  public function importCheckCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'relationship_type_id') === 0) {
        $newElement = $this->importTranslateRelationshipType($newElement);
      } elseif (is_array($element)) {
        $newElement = $this->importCheckCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  public function checkCiviElements(&$array) {
    $return = array();
    foreach($array as $key => $element) {
      $newKey = $key;
      $newElement = $element;
      if (strpos($key, 'relationship_type_id') === 0) {
        $newElement = $this->exportTranslateRelationshipType($newElement);
      } elseif (is_array($element)) {
        $newElement = $this->checkCiviElements($array[$key]);
      }
      
      $return[$newKey] = $newElement;
    }
    return $return;
  }
  
  protected function importTranslateRelationshipType($element) {
    $return = array();
    foreach($element as $i => $j) {
      $matches = array();
      if (preg_match("/([ab])_#name#_(.*)/", $i, $matches)) {
        $k = $this->getRelationshipTypeId($matches[2]).'_'.$matches[1];
        $return[$k] = $k;
      } else {
        $return[$i] = $j;
      }
    }
    return $return;
  }
  
  protected function exportTranslateRelationshipType($element) {
    $return = array();
    foreach($element as $i => $j) {
      $matches = array();
      if (preg_match("/(\d+)_([ab])/", $i, $matches)) {
        $k = $matches[2]. '_#name#_'.$this->getRelationshipTypeName($matches[1]);
        $return[$k] = $k;
      } else {
        $return[$i] = $j;
      }
    }

    return $return;
  }
  
  protected function getRelationshipTypeName($relationship_type_id) {
    try {
      return civicrm_api3('RelationshipType', 'getvalue', array('id' => $relationship_type_id, 'return' => 'name_a_b'));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  
  protected function getRelationshipTypeId($name) {
    try {
      return civicrm_api3('RelationshipType', 'getvalue', array('name_a_b' => $name, 'return' => 'id'));
    } catch (Exception $ex) {
      //do nothing
    }
    return false;
  }
  

  
}