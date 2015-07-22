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
import java.util.Scanner;

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

  private static boolean legoModeIsVoxel = true;
  private static String outputXmlFileName = "lego.xml";
  private static String outputJsonFileName = "lego.json";

  public static void main(String[] args) throws Exception {
    String modelName = getModelName(args);
    if (modelName == null) {
      System.err.println("No model name provided");
    }

    String[] modelNameSplit = modelName.split("\\.");
    if (modelNameSplit.length < 2) {
      System.err.println("Model name does not have an extension");
    }
    if (modelNameSplit[1].equals("bricks")) {
      legoModeIsVoxel = false;
    }

    String lego_model = readFile(modelName);
    String lego_model_file_name = getLegoModelFileName();
    lego_model += "\n" + readFile(lego_model_file_name);

    System.out.println("Reading model at: " + modelName);
    System.out.println("Reading base model at: " + lego_model_file_name);

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

    // Iterate through the solutions
    assert sol.satisfiable();
    int solution_count = 1;
    while (sol.satisfiable()) {
      sol.writeXML(outputXmlFileName);
      if (legoModeIsVoxel) {
        output_voxel_solution_json(world, sol);
      } else {
        output_brick_solution_json(world, sol);
      }
      System.out.println("Solution #" + String.valueOf(solution_count) + " generated. Press enter for next solution.");
      Scanner keyboard = new Scanner(System.in);
      keyboard.nextLine();

      A4Solution nextSolution = sol.next();
      if (nextSolution == sol) {
        System.out.println("No additional solutions found.");
        break;
      }
      sol = nextSolution;
      solution_count += 1;
    }
  }

  private static void output_voxel_solution_json(Module world, A4Solution sol) throws Exception {
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

    Map brickColourList = new LinkedHashMap();
    Expr brickColourExpr = CompUtil.parseOneExpression_fromString(world, "Brick<:colour");
    itr = ((A4TupleSet)sol.eval(brickColourExpr)).iterator();
    while (itr.hasNext()) {
      A4Tuple element = (A4Tuple)itr.next();
      brickColourList.put(element.atom(0), element.atom(1));
    }

    Map list = new LinkedHashMap();
    list.put("bricks", brickList);
    list.put("voxels", voxels);
    list.put("colours", brickColourList);
    StringWriter listOut = new StringWriter();
    JSONValue.writeJSONString(list, listOut);
    String listJSON = listOut.toString();

    PrintWriter out = new PrintWriter(outputJsonFileName);
    out.println(listJSON);
    out.close();
  }

  private static void output_brick_solution_json(Module world, A4Solution sol) throws Exception {
    PrintWriter out = new PrintWriter(outputJsonFileName);
    out.println("");
    out.close();
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

  private static String getLegoModelFileName() {
    if (legoModeIsVoxel) {
      return "models/voxels/lego.als";
    }
    return "models/bricks/lego.als";
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
