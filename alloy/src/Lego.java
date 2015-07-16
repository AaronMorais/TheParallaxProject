import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Iterator;
import org.json.simple.JSONArray;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.Map;
import org.json.simple.JSONValue;
import java.io.PrintWriter;
import java.lang.Thread;

import edu.mit.csail.sdg.alloy4.A4Reporter;
import edu.mit.csail.sdg.alloy4.XMLNode;
import edu.mit.csail.sdg.alloy4compiler.ast.Command;
import edu.mit.csail.sdg.alloy4compiler.ast.Expr;
import edu.mit.csail.sdg.alloy4compiler.ast.Module;
import edu.mit.csail.sdg.alloy4compiler.parser.CompUtil;
import edu.mit.csail.sdg.alloy4compiler.translator.A4Options;
import edu.mit.csail.sdg.alloy4compiler.translator.A4Solution;
import edu.mit.csail.sdg.alloy4compiler.translator.A4SolutionReader;
import edu.mit.csail.sdg.alloy4compiler.translator.A4Tuple;
import edu.mit.csail.sdg.alloy4compiler.translator.A4TupleSet;
import edu.mit.csail.sdg.alloy4compiler.translator.TranslateAlloyToKodkod;

public class Lego {
  private static String lego_model_file_name = "lego.als";
  private static String output_xml_file_name = "lego.xml";
  private static String output_json_file_name = "lego.json";

  public static void main(String[] args) throws Exception {
    String modelName = getModelName(args);
    if (modelName == null) {
      System.err.println("No model name provided");
    }

    String lego_model = readFile(modelName);
    lego_model += "\n" + readFile(lego_model_file_name);

    // TODO: Fully understand the black box below.
    A4Reporter rep = new A4Reporter();
    File tmpAls = File.createTempFile("alloyEvaluator", ".als");
    tmpAls.deleteOnExit();
    flushModelToFile(lego_model, tmpAls);
    Module world = CompUtil.parseEverything_fromFile(rep, null, tmpAls.getAbsolutePath());
    A4Options opt = new A4Options();
    opt.originalFilename = tmpAls.getAbsolutePath();
    opt.solver = A4Options.SatSolver.SAT4J;
    Command cmd = world.getAllCommands().get(0);
    A4Solution sol = TranslateAlloyToKodkod.execute_commandFromBook(rep, world.getAllReachableSigs(), cmd, opt);
    assert sol.satisfiable();
    sol.writeXML(output_xml_file_name);

    // Parsing the solution
    Map voxels = new LinkedHashMap();
    Expr xVoxel = CompUtil.parseOneExpression_fromString(world, "Voxel<:x");
    Iterator<A4Tuple> itr = ((A4TupleSet)sol.eval(xVoxel)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      voxels.put(element.atom(0), new LinkedHashMap());
    }
    itr = ((A4TupleSet)sol.eval(xVoxel)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      ((LinkedHashMap)voxels.get(element.atom(0))).put("x", element.atom(1));
    }

    Expr yVoxel = CompUtil.parseOneExpression_fromString(world, "Voxel<:y");
    itr = ((A4TupleSet)sol.eval(yVoxel)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      ((LinkedHashMap)voxels.get(element.atom(0))).put("y", element.atom(1));
    }

    Expr zVoxel = CompUtil.parseOneExpression_fromString(world, "Voxel<:z");
    itr = ((A4TupleSet)sol.eval(zVoxel)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      ((LinkedHashMap)voxels.get(element.atom(0))).put("z", element.atom(1));
    }

    Map brickList = new LinkedHashMap();
    Expr brickExpr = CompUtil.parseOneExpression_fromString(world, "Brick");
    itr = ((A4TupleSet)sol.eval(brickExpr)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      brickList.put(element.atom(0), new LinkedList());
    }

    Expr voxelToBrickExpr = CompUtil.parseOneExpression_fromString(world, "Brick<:voxels");
    itr = ((A4TupleSet)sol.eval(voxelToBrickExpr)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      ((LinkedList)brickList.get(element.atom(0))).add(element.atom(1));
    }

    Map list = new LinkedHashMap();
    list.put("bricks", brickList);
    list.put("voxels", voxels);
    StringWriter listOut = new StringWriter();
    JSONValue.writeJSONString(list, listOut);
    String listJSON = listOut.toString();

    PrintWriter out = new PrintWriter(output_json_file_name);
    out.println(listJSON);
    out.close();

    System.out.println("Displaying result at localhost:3000");
    Process p = Runtime.getRuntime().exec("python -m SimpleHTTPServer 3000");
    Process p2 = Runtime.getRuntime().exec("open localhost:3000");
    p2.waitFor();
    p.waitFor();
  }

  private static void flushModelToFile(String model, File tmpAls) throws IOException {
    BufferedOutputStream bos = null;
    try {
      bos = new BufferedOutputStream(new FileOutputStream(tmpAls));
      bos.write(model.getBytes());
      bos.flush();
    } finally {
      if (bos != null)
          bos.close();
    }
  }

  private static String getModelName(String[] args) {
    if (args[0].equals("-model")) {
      if (1 < args.length) {
        return args[1];
      } else {
        System.err.println("-model requires a model name");
      }
    }
    return null;
  }

  private static String readFile(String fileName) throws IOException {
    BufferedReader br = new BufferedReader(new FileReader(fileName));
    try {
        StringBuilder sb = new StringBuilder();
        String line = br.readLine();

        while (line != null) {
            sb.append(line);
            sb.append("\n");
            line = br.readLine();
        }
        return sb.toString();
    } finally {
        br.close();
    }
  }
}
